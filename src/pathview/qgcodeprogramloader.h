/****************************************************************************
**
** Copyright (C) 2014 Alexander Rössler
** License: LGPL version 2.1
**
** This file is part of QtQuickVcp.
**
** All rights reserved. This program and the accompanying materials
** are made available under the terms of the GNU Lesser General Public License
** (LGPL) version 2.1 which accompanies this distribution, and is available at
** http://www.gnu.org/licenses/lgpl-2.1.html
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
** Contributors:
** Alexander Rössler @ The Cool Tool GmbH <mail DOT aroessler AT gmail DOT com>
**
****************************************************************************/

#ifndef QGCODEPROGRAMLOADER_H
#define QGCODEPROGRAMLOADER_H

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include "qgcodeprogrammodel.h"

class QGCodeProgramLoader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString localFilePath READ localFilePath WRITE setLocalFilePath NOTIFY localFilePathChanged)
    Q_PROPERTY(QString localPath READ localPath WRITE setLocalPath NOTIFY localPathChanged)
    Q_PROPERTY(QString remotePath READ remotePath WRITE setRemotePath NOTIFY remotePathChanged)
    Q_PROPERTY(QGCodeProgramModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
    explicit QGCodeProgramLoader(QObject *parent = 0);

    QString text() const
    {
        return m_text;
    }

    QString localFilePath() const
    {
        return m_localFilePath;
    }

    QString localPath() const
    {
        return m_localPath;
    }

    QString remotePath() const
    {
        return m_remotePath;
    }

    QGCodeProgramModel * model() const
    {
        return m_model;
    }

signals:
    void textChanged();
    void localFilePathChanged(QString arg);
    void localPathChanged(QString arg);
    void remotePathChanged(QString arg);
    void modelChanged(QGCodeProgramModel * arg);
    void loadingFinished();
    void loadingFailed();
    void error(QString message);

public slots:
    void save(const QString &text);
    void saveAs(const QString &localFilePath, const QString &text);
    void load();

    void setText(const QString &arg) {
        if (m_text != arg) {
            m_text = arg;
            emit textChanged();
        }
    }

    void setLocalFilePath(QString arg)
    {
        if (m_localFilePath != arg) {
            m_localFilePath = arg;
            emit localFilePathChanged(arg);
        }
    }

    void setLocalPath(QString arg)
    {
        if (m_localPath == arg)
            return;

        m_localPath = arg;
        emit localPathChanged(arg);
    }

    void setRemotePath(QString arg)
    {
        if (m_remotePath == arg)
            return;

        m_remotePath = arg;
        emit remotePathChanged(arg);
    }

    void setModel(QGCodeProgramModel * arg)
    {
        if (m_model != arg) {
            m_model = arg;
            emit modelChanged(arg);
        }
    }

private:
    QString m_localFilePath;
    QString m_localPath;
    QString m_remotePath;
    QGCodeProgramModel * m_model;
    QString m_text;
};

#endif // QGCODEPROGRAMLOADER_H
