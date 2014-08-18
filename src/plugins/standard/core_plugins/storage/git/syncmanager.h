/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2012-2014 by Alexander Mikhalov                         *
 *   alexander.mikhalov@gmail.com                                          *
 *                                                                         *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      *
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/
#ifndef SYNCMANAGER_H
#define SYNCMANAGER_H

#include <QObject>
#include <QMutex>

class SyncManager : public QObject
{
    Q_OBJECT
public:
    explicit SyncManager(const QString & gitPath = "git", QObject *parent = 0);

    void setGitPath(const QString &gitPath);

    bool pull(const QString &localDir);
    bool push(const QString & localDir);
    bool isLocalRepoExists(const QString & localDir);
    bool clone(const QString &remoteUrl, const QString & localDir);

    bool isActive();
    void setActive(bool b);

    QString fullOutput() const;
    QString stdOutput() const;
    QString stdError() const;
    QString output() const;

    static bool checkBinary(const QString & gitPath, QString *version = 0);

signals:
    
public slots:

private:
    QMutex m_mutex;
    bool m_isActive;
    QString m_gitPath;
    QString m_output;
    QString m_stdError;
    QString m_stdOutput;
};

#endif // SYNCMANAGER_H
