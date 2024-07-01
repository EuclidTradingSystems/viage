#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QLocale>
#include <QTranslator>

#include <client.hpp>
#include <netManager.hpp>
#include <bridge.hpp>

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    if (!client::tempDir.isValid())
    {
        qDebug() << "temp directory error :" << client::tempDir.errorString();
        return 4;
    }

    QTranslator translator;
    if (client_utils::is_german())
    {
        if (translator.load("viage_de", ":/qm_files/"))
            app.installTranslator(&translator);
    }

    // if (translator.load(QLocale(), "viage", "_", ":/qm_files/"))
    //     app.installTranslator(&translator);

    using namespace Interface;

    bridge::instance().init();

#ifndef EMSCRIPTEN
    qDebug() << "Device supports OpenSSL: " << QSslSocket::supportsSsl();

    // QString host{"https://viage.euclidtradingsystems.com"};
   QString host{"https://viagetestrive.euclidtradingsystems.com"};
//    QString host{"http://127.0.0.0:8080"};

    for (int i = 0; i < argc; i++)
        if (QString::compare(argv[i], "--host") == 0)
        {
            host = argv[i + 1];
            break;
        }

    qDebug() << "Host :" << host;


    netManager::instance().init(host,
                                "auth",
                                "format=json&jsconfig=TreatEnumAsInteger");
#else
    netManager::instance().init("auth",
                                "format=json&jsconfig=TreatEnumAsInteger");
#endif

    client::instance().init();

    // qml engine
    const QUrl url(QStringLiteral("qrc:/ui/main.qml"));
    QObject::connect(bridge::instance().engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url]
        (QObject* obj, const QUrl &objUrl)
        {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
            else
                bridge::instance().setQmlObject(obj);
        }, Qt::QueuedConnection);

    bridge::instance().engine->load(url);

    return app.exec();
}
