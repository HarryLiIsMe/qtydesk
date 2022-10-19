#ifndef LOG4J_H
#define LOG4J_H

#include <QObject>

class Log4j: public QObject
{
    Q_OBJECT

public:
    Log4j();
public :
    static const int CONSOLE =0;
    static const int FILE =1;
    static QString getPattern();
    static void userLog(int conOrFile);
    static void messageOutputConsole(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static void messageOutputToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg);

};

#endif // LOG4J_H
