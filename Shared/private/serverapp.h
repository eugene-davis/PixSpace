#ifndef APP_H
#define APP_H

#include <QObject>
#include <QSocketNotifier>
#include <QElapsedTimer>
#include <QTimer>

#include "datamanager.h"
#include "universe.h"
#include "shared_global.h"

class SHAREDSHARED_EXPORT ServerApp : public QObject
{
    Q_OBJECT
public:
    explicit ServerApp(QObject *parent = 0);

    // Unix signal handlers, called by system.
    static void hupSignalHandler(int unused);
    static void termSignalHandler(int unused);
    static void intSignalHandler(int unused);

signals:
    /**
     * @brief finished Emitted when simulation stops
     */
    void finished();

public slots:
    /**
     * @brief tick Tick the simulation
     */
    void tick();

    /**
     * @brief start Start ticking the simulation
     */
    void start();

    /**
     * @brief stop Stop ticking the application and emit finished()
     */
    void stop();

    /**
     * @brief handleSigHup Called when SIGHUP occurs
     */
    void handleSigHup();

    /**
     * @brief handleSigTerm Called when SIGTERM occurs
     */
    void handleSigTerm();

    /**
     * @brief handleSigInt Called when SIGINT occurs
     */
    void handleSigInt();

private:
    // Sockets
    static int sighupFd[2];
    static int sigtermFd[2];
    static int sigintFd[2];

    QSocketNotifier *snHup;
    QSocketNotifier *snTerm;
    QSocketNotifier *snInt;

    // Data structures
    Universe *universe;
    DataManager *datamanager;

    // Timers
    QElapsedTimer timer;
    QTimer tickTimer;
};

#endif // APP_H