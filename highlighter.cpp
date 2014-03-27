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

#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent), m_max(-1)
{

}

void Highlighter::update()
{
    rehighlight();
}

void Highlighter::highlightBlock(const QString &text)
{
    QTextCharFormat redFormat;
    redFormat.setForeground(Qt::red);
    redFormat.setFontWeight(QFont::Bold);
    redFormat.setFontUnderline(true);
    QTextCharFormat magentaFormat;
    magentaFormat.setForeground(Qt::darkMagenta);
    magentaFormat.setFontWeight(QFont::Bold);
    magentaFormat.setFontUnderline(true);

    QString line = text;
    line.replace(","," ");
    line.replace(";"," ");
    line.replace("."," ");
    line.replace("\n"," ");
    line.replace("\t"," ");

    QStringList list = line.split(" ");

    int begin = 0;
    foreach(QString word, list){
        if(word.isEmpty()){
            begin++;
            continue;
        }

        int size = word.length();

        bool ok;
        int val = word.toInt(&ok);

        if(!ok){
            setFormat(begin, size, redFormat);
        }else{
            if(val<0 || val>m_max)
                setFormat(begin, size, redFormat);
            if(m_list.contains(val))
                setFormat(begin, size, magentaFormat);
        }

        begin += word.length() + 1;
    }
}

void Highlighter::setMaximum(int max)
{
    m_max = max;
}

void Highlighter::setForbiddenValues(QList<int> list)
{
    m_list = list;
}
