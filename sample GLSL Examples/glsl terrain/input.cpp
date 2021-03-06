//-----------------------------------------------------------------------------
// Copyright (c) 2007 dhpoware. All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

// Forces the Win32 Raw Input Model APIs to be visible.
// Note that the minimum supported OS for these APIs is Windows XP.
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0501

#include <windows.h>
#include "input.h"

//-----------------------------------------------------------------------------
// Keyboard.
//-----------------------------------------------------------------------------

Keyboard &Keyboard::instance()
{
    static Keyboard theInstance;
    return theInstance;
}

Keyboard::Keyboard()
{
    m_lastChar = 0;
    m_pCurrKeyStates = m_keyStates[0];
    m_pPrevKeyStates = m_keyStates[1];

    memset(m_pCurrKeyStates, 0, 256);
    memset(m_pPrevKeyStates, 0, 256);
}

Keyboard::~Keyboard() {}

void Keyboard::handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CHAR:
        m_lastChar = static_cast<int>(wParam);
        break;

    default:
        break;
    }
}

void Keyboard::update()
{
    BYTE *pTempKeyStates = m_pPrevKeyStates;

    m_pPrevKeyStates = m_pCurrKeyStates;
    m_pCurrKeyStates = pTempKeyStates;

    GetKeyboardState(reinterpret_cast<BYTE*>(m_pCurrKeyStates));
}

//-----------------------------------------------------------------------------
// Mouse.
//-----------------------------------------------------------------------------

#if !defined(WHEEL_DELTA)
#define WHEEL_DELTA 120
#endif

#if !defined(WM_MOUSEWHEEL)
#define WM_MOUSEWHEEL 0x020A
#endif

const float Mouse::WEIGHT_MODIFIER = 0.5f;
BYTE Mouse::m_tempBuffer[TEMP_BUFFER_SIZE];

Mouse &Mouse::instance()
{
    static Mouse theInstance;
    return theInstance;
}

Mouse::Mouse()
{
    m_hWnd = 0;
    m_cursorVisible = true;
    m_filterMouse = true;

    m_wheelDelta = 0;
    m_prevWheelDelta = 0;
    m_mouseWheel = 0.0f;

    m_xPosAbsolute = 0;
    m_yPosAbsolute = 0;
    m_xPosRelative = 0;
    m_yPosRelative = 0;
       
    m_weightModifier = WEIGHT_MODIFIER;
    m_historyBufferSize = HISTORY_BUFFER_SIZE;
}

Mouse::~Mouse()
{
    detach();
}

bool Mouse::attach(HWND hWnd)
{
    if (!hWnd)
        return false;

    if (!m_hWnd)
    {
        RAWINPUTDEVICE rid[1] = {0};

        rid[0].usUsagePage = 1;
        rid[0].usUsage = 2;
        rid[0].dwFlags = 0;
        rid[0].hwndTarget = hWnd;

        if (!RegisterRawInputDevices(rid, 1, sizeof(rid[0])))
            return false;

        m_hWnd = hWnd;
    }

    if (!m_cursorVisible)
        hideCursor(true);

    m_filtered[0] = 0.0f;
    m_filtered[1] = 0.0f;
    m_pCurrButtonStates = m_buttonStates[0];
    m_pPrevButtonStates = m_buttonStates[1];

    m_mouseIndex = 0;
    m_mouseMovementX[0] = m_mouseMovementX[1] = 0.0f;
    m_mouseMovementY[0] = m_mouseMovementY[1] = 0.0f;

    memset(m_history, 0, sizeof(m_history));
    memset(m_buttonStates, 0, sizeof(m_buttonStates));

    return true;
}

void Mouse::detach()
{
    if (!m_cursorVisible)
    {
        hideCursor(false);
        
        // Save the cursor visibility state in case attach() is called later.
        m_cursorVisible = false;
    }

    m_hWnd = 0;
}

void Mouse::performMouseFiltering(float x, float y)
{
    // Filter the relative mouse movement based on a weighted sum of the mouse
    // movement from previous frames to ensure that the mouse movement this
    // frame is smooth.
    //
    // For further details see:
    //  Nettle, Paul "Smooth Mouse Filtering", flipCode's Ask Midnight column.
    //  http://www.flipcode.com/cgi-bin/fcarticles.cgi?show=64462

    // Newer mouse entries towards front and older mouse entries towards end.
    for (int i = m_historyBufferSize - 1; i > 0; --i)
    {
        m_history[i * 2] = m_history[(i - 1) * 2];
        m_history[i * 2 + 1] = m_history[(i - 1) * 2 + 1];
    }

    // Store current mouse entry at front of array.
    m_history[0] = x;
    m_history[1] = y;

    float averageX = 0.0f;
    float averageY = 0.0f;
    float averageTotal = 0.0f;
    float currentWeight = 1.0f;

    // Filter the mouse.
    for (int i = 0; i < m_historyBufferSize; ++i)
    {
        averageX += m_history[i * 2] * currentWeight;
        averageY += m_history[i * 2 + 1] * currentWeight;
        averageTotal += 1.0f * currentWeight;
        currentWeight *= m_weightModifier;
    }

    m_filtered[0] = averageX / averageTotal;
    m_filtered[1] = averageY / averageTotal;
}

void Mouse::performMouseSmoothing(float x, float y)
{
    // Smooth out the mouse movement by averaging the distance the mouse
    // has moved over a couple of frames.

    m_mouseMovementX[m_mouseIndex] = x;
    m_mouseMovementY[m_mouseIndex] = y;

    m_filtered[0] = (m_mouseMovementX[0] + m_mouseMovementX[1]) * 0.5f;
    m_filtered[1] = (m_mouseMovementY[0] + m_mouseMovementY[1]) * 0.5f;

    m_mouseIndex ^= 1;
    m_mouseMovementX[m_mouseIndex] = 0.0f;
    m_mouseMovementY[m_mouseIndex] = 0.0f;
}

void Mouse::handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    RAWINPUT *pRaw = 0;
    UINT size = TEMP_BUFFER_SIZE;

    switch (msg)
    {
    default:
        break;

    case WM_INPUT:
        GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT,
            m_tempBuffer, &size, sizeof(RAWINPUTHEADER));

        pRaw = reinterpret_cast<RAWINPUT*>(m_tempBuffer);

        if (pRaw->header.dwType == RIM_TYPEMOUSE)
        {
            m_xPosRelative = static_cast<float>(pRaw->data.mouse.lLastX);
            m_yPosRelative = static_cast<float>(pRaw->data.mouse.lLastY);

            if (m_filterMouse)
            {
                performMouseFiltering(m_xPosRelative, m_yPosRelative);

                m_xPosRelative = m_filtered[0];
                m_yPosRelative = m_filtered[1];

                performMouseSmoothing(m_xPosRelative, m_yPosRelative);

                m_xPosRelative = m_filtered[0];
                m_yPosRelative = m_filtered[1];
            }
        }

        break;

    case WM_MOUSEMOVE:
        m_xPosAbsolute = static_cast<int>(static_cast<short>(LOWORD(lParam)));
        m_yPosAbsolute = static_cast<int>(static_cast<short>(HIWORD(lParam)));
        break;

    case WM_MOUSEWHEEL:
        m_wheelDelta += static_cast<int>(static_cast<int>(wParam) >> 16);
        break;
    }
}

void Mouse::hideCursor(bool hide)
{
    if (hide)
    {
        m_cursorVisible = false;

        while (ShowCursor(FALSE) >= 0)
            ; // do nothing
    }
    else
    {
        m_cursorVisible = true;

        while (ShowCursor(TRUE) < 0)
            ; // do nothing
    }
}

void Mouse::setPosition(UINT x, UINT y)
{
    POINT pt = {x, y};

    if (ClientToScreen(m_hWnd, &pt))
    {
        SetCursorPos(pt.x, pt.y);

        m_xPosAbsolute = x;
        m_yPosAbsolute = y;

        m_xPosRelative = 0.0f;
        m_yPosRelative = 0.0f;
    }
}

void Mouse::smoothMouse(bool smooth)
{
    m_filterMouse = smooth;
}

void Mouse::update()
{
    bool *pTempMouseStates = m_pPrevButtonStates;

    m_pPrevButtonStates = m_pCurrButtonStates;
    m_pCurrButtonStates = pTempMouseStates;

    m_pCurrButtonStates[0] = (GetKeyState(VK_LBUTTON) & 0x8000) ? true : false;
    m_pCurrButtonStates[1] = (GetKeyState(VK_RBUTTON) & 0x8000) ? true : false;
    m_pCurrButtonStates[2] = (GetKeyState(VK_MBUTTON) & 0x8000) ? true : false;

    m_mouseWheel = static_cast<float>(m_wheelDelta - m_prevWheelDelta) / static_cast<float>(WHEEL_DELTA);
    m_prevWheelDelta = m_wheelDelta;
}