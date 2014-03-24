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
