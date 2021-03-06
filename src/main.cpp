#include <vector>

#include <QGuiApplication>

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QLocale>
#include <QTranslator>

#include <netManager.hpp>
#include <bridge.hpp>
#include <wrapped_nested_list.hpp>
#include <Models/list_model.hpp>
#include <Models/account_filter_model.hpp>
#include <Models/user_filter_model.hpp>
#include <Models/document_filter_model.hpp>
#include <Items/user_item.hpp>
#include <Items/account_item.hpp>
#include <Items/owner_item.hpp>
#include <Items/contact_item.hpp>
#include <Items/habitat_item.hpp>
#include <Items/exterior_item.hpp>
#include <Items/document_item.hpp>
#include <wrapped_calculator.hpp>

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    qDebug() << "Device supports OpenSSL: " << QSslSocket::supportsSsl();

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "viage_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QQmlApplicationEngine engine;
    auto context = engine.rootContext();

    using namespace Data;
    using namespace People;

    QString host{"https://viagetestrive.euclidtradingsystems.com"};

    for (int i = 0; i < argc; i++)
        if (QString::compare(argv[i], "--host") == 0)
        {
            host = argv[i + 1];
            break;
        }

    qDebug() << "Host :" << host;

    using namespace Interface;

    netManager manager{host,
                "auth",
                "format=json&jsconfig=TreatEnumAsInteger"};

    using namespace Wrapper;

    // calculator
    Calculator::wrapped_calculator calculator{&manager, context};
    qmlRegisterType<list_model<senior_citizen_item>>("People", 1, 0, "SeniorCitizenModel");

    // accounts
    wrapped_list<item_list<account_item>>
            wrapped_accounts{&manager, context};
    wrapped_accounts.makeConnections();

    list_model<account_item> accountModel{};
    accountModel.setList(wrapped_accounts.get_inner());

    account_filter_model accountFilter{&accountModel};
    qmlRegisterUncreatableType<account_filter_model>("Data", 1, 0, "AccountModel", "");
    context->setContextProperty("accountModel", &accountFilter);

    // owners
    wrapped_nested_list<item_list<owner_item>, account_item>
            wrapped_owners{&manager, wrapped_accounts.get_inner(), context};
    qmlRegisterType<list_model<owner_item>>("People", 1, 0, "OwnersModel");

    // contacts
    wrapped_nested_list<item_list<contact_item>, account_item>
            wrapped_contacts{&manager, wrapped_accounts.get_inner(), context};
    qmlRegisterType<list_model<contact_item>>("People", 1, 0, "ContactModel");

    using namespace Places;

    // habitat
    wrapped_nested_item<habitat_item, account_item>
            wrapped_habitat{&manager, context};
    wrapped_habitat.makeConnections(wrapped_accounts.get_inner());
    // exterior
    wrapped_nested_item<exterior_item, account_item>
            wrapped_exterior{&manager, context};
    wrapped_exterior.makeConnections(wrapped_accounts.get_inner());

    // documents
    wrapped_nested_list<item_list<document_item>, account_item>
            wrapped_documents{&manager, wrapped_accounts.get_inner(), context};
    qmlRegisterType<list_model<document_item>>("Data", 1, 0, "DocumentModel");
    qmlRegisterType<document_filter_model>("Data", 1, 0, "DocumentFilterModel");

    // users
    wrapped_list<item_list<user_item>>
            wrapped_users{&manager, context};
    wrapped_users.makeConnections();

    list_model<user_item> userModel{};
    userModel.setList(wrapped_users.get_inner());

    user_filter_model userFilter{&userModel, false};
    user_filter_model selectedUser{&userModel, true};
    qmlRegisterUncreatableType<user_filter_model>("People", 1, 0, "UserModel", "");
    context->setContextProperty("userModel", &userFilter);
    context->setContextProperty("selectedUser", &selectedUser);

    bridge bridge{&manager,
                wrapped_users.get_inner(),
                wrapped_accounts.get_inner(),
                wrapped_documents.get_inner()};

    qmlRegisterUncreatableType<Interface::bridge>("Interface", 1, 0, "Bridge", "");
    context->setContextProperty("bridge", &bridge);

    QObject::connect(&manager, &Interface::netManager::loggedIn,
                     [&wrapped_accounts, &bridge, &wrapped_users](const bool& success)
    {
        if (success)
        {
            wrapped_accounts.get();
            if (bridge.getClearance() == user_item::Administrator)
                wrapped_users.get();
        }
    });

    // Onboarding
    QObject::connect(&bridge,
                     &bridge::requestOwners,
                     wrapped_owners.get_inner(),
                     &item_list<owner_item>::loadFrom);

    // Hiering
    QObject::connect(&bridge,
                     &bridge::requestUser,
                     &selectedUser,
                     &user_filter_model::setFilterRole);

    // qml engine
    const QUrl url(QStringLiteral("qrc:/ui/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [&bridge, url](QObject* obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
        else
            bridge.setQmlObject(obj);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
