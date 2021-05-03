#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>
#include <KIO/AccessManager>
#include <KLocalizedContext>
#include <KLocalizedString>

#include "feed/feedrepository.h"
#include "feed/yahoofinancefeed.h"

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("cartera");
    QCoreApplication::setOrganizationDomain("librehat.com");
    QCoreApplication::setApplicationName("Cartera");

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
    
    // FIXME
    cartera::FeedRepository repo(nullptr);
    KIO::AccessManager *acm = new KIO::AccessManager(nullptr);
    cartera::YahooFinanceFeed yhFeed(acm);
    repo.addFeedHandler("YH", &yhFeed);

    return app.exec();
}
