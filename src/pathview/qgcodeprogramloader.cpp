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

#include "qgcodeprogramloader.h"

QGCodeProgramLoader::QGCodeProgramLoader(QObject *parent) :
    QObject(parent),
    m_localFilePath(""),
    m_localPath(""),
    m_remotePath(""),
    m_model(nullptr)
{
}

void QGCodeProgramLoader::save(const QString &text)
{
    QString localFilePath = QUrl(m_localFilePath).toLocalFile();
    saveAs(localFilePath, text);
}

void QGCodeProgramLoader::saveAs(const QString &localFilePath, const QString &text)
{
    QFile f(localFilePath);
    if (!f.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) {
        emit error(tr("Cannot save: ") + f.errorString());
        return;
    }
    f.write(text.toLocal8Bit());
    f.close();
}

void QGCodeProgramLoader::load()
{
    if (m_model == nullptr)
    {
        emit loadingFailed();
        return;
    }

    QString localFilePath = QUrl(m_localFilePath).toLocalFile();
    QString localPath = QUrl(m_localPath).toLocalFile();
    QString remotePath = QUrl(m_remotePath).toLocalFile();
    QString remoteFilePath;

    if (localFilePath.indexOf(localPath) == 0)
    {
        remoteFilePath = QDir(remotePath).filePath(localFilePath.mid(localPath.length() + 1));
    }
    else
    {
        QFileInfo fileInfo(localFilePath);
        remoteFilePath = QDir(remotePath).filePath(fileInfo.fileName());
    }

    QFile file(localFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit loadingFailed();
        return;
    }

    QString data = QString(file.readAll());
    setText(data);

    int lineNumber = 0;
    lineNumber = data.count(QLatin1Char('\n')) + 1; // +1 for the last line

    m_model->beginUpdate();
    m_model->prepareFile(remoteFilePath, lineNumber);

    lineNumber = 0;
    file.reset();
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        lineNumber++;
        m_model->setData(remoteFilePath, lineNumber, QString(line), QGCodeProgramModel::GCodeRole);
    }

    m_model->endUpdate();

    file.close();
    emit loadingFinished();
}
