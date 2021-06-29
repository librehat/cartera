#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>
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

    QScopedPointer<cartera::Backend> backend(new cartera::Backend());

    QQmlApplicationEngine engine;
    qmlRegisterSingletonInstance("com.librehat.cartera", 1, 0, "Backend", backend.get());

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
    
    return app.exec();
}
