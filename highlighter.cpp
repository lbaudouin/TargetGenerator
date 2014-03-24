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
