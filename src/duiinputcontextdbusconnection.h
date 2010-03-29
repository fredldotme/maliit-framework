/* * This file is part of dui-im-framework *
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * Contact: Nokia Corporation (directui@nokia.com)
 *
 * If you have questions regarding the use of this file, please contact
 * Nokia at directui@nokia.com.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * and appearing in the file LICENSE.LGPL included in the packaging
 * of this file.
 */

#ifndef DUIINPUTCONTEXTDBUSCONNECTION_H
#define DUIINPUTCONTEXTDBUSCONNECTION_H

#include <QDBusContext>
#include <QDBusObjectPath>
#include <QMap>
#include <QEvent>

#include "duiinputcontextconnection.h"

class QPoint;
class QString;
class QRegion;
class QVariant;

class DuiInputContextDBusConnectionPrivate;

Q_DECLARE_METATYPE(QRect);

/*!
  \brief DuiInputContextDBusConnection handles input context connection for DBus based
  input context
 */
class DuiInputContextDBusConnection: public DuiInputContextConnection,
                                     protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.maemo.duiinputmethodserver1")

public:
    DuiInputContextDBusConnection();
    virtual ~DuiInputContextDBusConnection();

    //! Returns true if the connection to D-Bus is valid and the object is properly usable
    bool isValid();

    //! \reimp
    virtual void sendPreeditString(const QString &string, PreeditFace preeditFace = PreeditDefault);
    virtual void sendCommitString(const QString &string);
    virtual void sendKeyEvent(const QKeyEvent &keyEvent);
    virtual void notifyImInitiatedHiding();

    virtual int contentType(bool &valid);
    virtual bool correctionEnabled(bool &valid);
    virtual bool predictionEnabled(bool &valid);
    virtual bool autoCapitalizationEnabled(bool &valid);
    virtual void setGlobalCorrectionEnabled(bool);
    virtual bool surroundingText(QString &text, int &cursorPosition);
    virtual bool hasSelection(bool &valid);
    virtual int inputMethodMode(bool &valid);
    virtual QRect preeditRectangle(bool &valid);
    virtual void setRedirectKeys(bool enabled);
    virtual void copy();
    virtual void paste();
    //! \reimp_end

public slots:
    // communication w/ duiinputcontext

    //void setContextObject(QDBusObjectPath callbackObject);
    void setContextObject(QString callbackObject);

    //! sets the input to go to calling connection
    void activateContext();

    //! ipc method provided to the application, shows input method
    void showInputMethod();

    //! ipc method provided to the application, hides input method
    void hideInputMethod();

    //! ipc method provided to the application, signals mouse click on preedit
    void mouseClickedOnPreedit(const QPoint &pos, const QRect &preeditRect);

    //! ipc method provided to the application, sets preedit
    void setPreedit(const QString &text);

    void updateWidgetInformation(const QMap<QString, QVariant> &stateInformation);

    //! ipc method provided to the application, resets the input method
    void reset();

    /*!
     * \brief Target application changed orientation
     */
    void appOrientationChanged(int angle);

    virtual void updateInputMethodArea(const QRegion &region);

    /*! \brief Set copy/paste state for appropriate UI elements in the input method server
     *  \param copyAvailable bool TRUE if text is selected
     *  \param pasteAvailable bool TRUE if clipboard content is not empty
     */
    virtual void setCopyPasteState(bool copyAvailable, bool pasteAvailable);

    /*!
     * \brief Process a key event redirected from hardware keyboard to input method plugin(s).
     *
     * This is called only if one has enabled redirection by calling \a setRedirectKeys.
     */
    void processKeyEvent(QEvent::Type keyType, Qt::Key keyCode, Qt::KeyboardModifiers modifiers,
                         const QString &text, bool autoRepeat, int count, int nativeScanCode);

private:
    Q_DISABLE_COPY(DuiInputContextDBusConnection)

    DuiInputContextDBusConnectionPrivate *d;
};


#endif