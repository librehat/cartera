#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>
#include <QQuickStyle>
#include <KLocalizedContext>
#include <KLocalizedString>

#include "backend.h"

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("cartera");
    QCoreApplication::setOrganizationDomain("librehat.com");
    QCoreApplication::setApplicationName("Cartera");

#ifdef Q_OS_WINDOWS
    QQuickStyle::setStyle("Universal");
#endif

    QQmlApplicationEngine engine;
    qmlRegisterSingletonType<cartera::Backend>(
        "com.librehat.cartera",
        1,
        0,
        "Backend",
        [](QQmlEngine *engine, QJSEngine *jsEngine) -> QObject* {
            return new cartera::Backend(jsEngine, engine);
        });

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
    
    return app.exec();
}
