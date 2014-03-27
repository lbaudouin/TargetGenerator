// Copyright 2014, Léo Baudouin
//
// This file is part of a free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
//
// This software is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details. You should have
// received a copy of the GNU General Public License along with
// this software. If not, see <http://www.gnu.org/licenses/>.

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QtGui>
#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>

#include <QDebug>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
        bool text;
    };

    Highlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text);

private:
    int m_max;
    QList<int> m_list;

public slots:
    void update();
    void setMaximum(int max);
    void setForbiddenValues(QList<int> list);

};

#endif
