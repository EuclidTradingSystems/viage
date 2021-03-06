#ifndef BRIDGE_HPP
#define BRIDGE_HPP

#include <QObject>
#include <QStandardPaths>
#include <QTemporaryDir>

#include <wobjectdefs.h>

#include "netManager.hpp"

#include <Items/user_item.hpp>

namespace Data
{
struct document_item;
struct account_item;

template <typename T>
class item_list;
}

namespace Interface
{
class bridge final : public QObject
{
    W_OBJECT(bridge)

public:
    bridge(Interface::netManager* manager,
           Data::item_list<Data::People::user_item>* users,
           Data::item_list<Data::account_item>* accounts,
           Data::item_list<Data::document_item>* documents);

    void onLogin(const bool& success, const QString& errorString) const;

    void onError(const QString& prefix,
                 const QString& errorString) const;

    void authenticate(const QString& username, const QString& password) const;
    W_INVOKABLE(authenticate, (const QString&, const QString&))

    void onboard();
    W_INVOKABLE(onboard)
    void hire();
    W_INVOKABLE(hire)

    void downloadFile(const QString& key,
                      const QUrl &directory,
                      const QString &fileName) const;
    W_INVOKABLE(downloadFile, (const QString&,
                               const QUrl&,
                               const QString&))

    void requestReport() const;
    W_INVOKABLE(requestReport)

    void updatePwd(const QString& newPwd) const;
    W_INVOKABLE(updatePwd)

    void lockUser(int id, const bool& locked) const;
    W_INVOKABLE(lockUser)

    void getAccountDates() const;
    W_INVOKABLE(getAccountDates)

    void updateState(int newState) const;
    W_INVOKABLE(updateState)

    void resetPwd(int id) const;
    W_INVOKABLE(resetPwd)

    QUrl getPictureName(QString& name, int index) const;
    W_INVOKABLE(getPictureName, (QString&, int))

    void setQmlObject(QObject* obj) noexcept { qmlObject = obj; }

    bool hasFlag(int value, int flag) const noexcept;
    W_INVOKABLE(hasFlag, (int, int))
    bool accountHasFlag(int flag) const noexcept;
    W_INVOKABLE(accountHasFlag, (int))

    void loaded() const
    W_SIGNAL(loaded)

    void requestUser(int id)
    W_SIGNAL(requestUser, id)

    void requestOwners(int id)
    W_SIGNAL(requestOwners, id)

    int getAccountId() const;
    void setAccoountId(int newAccountId);
    void accountIdChanged()
    W_SIGNAL(accountIdChanged)

    int getAccountState() const;
    void setAccountState(int newAccountState);
    void accountStateChanged()
    W_SIGNAL(accountStateChanged)

    bool getDocumentsCompleted() const;
    void setDocumentsCompleted(bool newDocumentsCompleted);
    void documentsCompletedChanged()
    W_SIGNAL(documentsCompletedChanged)

    Data::People::user_item::clearances getClearance() const;
    void setClearance(int newClearance);
    void clearanceChanged()
    W_SIGNAL(clearanceChanged)

    int getUserId() const;
    void setUserId(int newUserId);
    void userIdChanged()
    W_SIGNAL(userIdChanged)

    const QDate &getAccountReceived() const;
    void setAccountReceived(const QDate &newAccountReceived);
    void accountReceivedChanged()
    W_SIGNAL(accountReceivedChanged)

    const QDate &getAccountExpertized() const;
    void setAccountExpertized(const QDate &newAccountExpertized);
    void accountExpertizedChanged()
    W_SIGNAL(accountExpertizedChanged)

    const QDate &getAccountNotarized() const;
    void setAccountNotarized(const QDate &newAccountNotarized);
    void accountNotarizedChanged()
    W_SIGNAL(accountNotarizedChanged)

    const QDate &getAccountPaid() const;
    void setAccountPaid(const QDate &newAccountPaid);
    void accountPaidChanged()
    W_SIGNAL(accountPaidChanged)

    const QDate &getAccountDecided() const;
    void setAccountDecided(const QDate &newAccountDecided);
    void accountDecidedChanged()
    W_SIGNAL(accountDecidedChanged)

    W_PROPERTY(bool, documentsCompleted READ getDocumentsCompleted NOTIFY documentsCompletedChanged)
    W_PROPERTY(Data::People::user_item::clearances, clearance READ getClearance NOTIFY clearanceChanged)
    W_PROPERTY(int, userId READ getUserId NOTIFY userIdChanged)
    W_PROPERTY(int, accountId READ getAccountId WRITE setAccoountId NOTIFY accountIdChanged)
    W_PROPERTY(int, accountState READ getAccountState WRITE setAccountState NOTIFY accountStateChanged)
    W_PROPERTY(QDate, accountReceived READ getAccountReceived NOTIFY accountReceivedChanged)
    W_PROPERTY(QDate, accountExpertized READ getAccountExpertized NOTIFY accountExpertizedChanged)
    W_PROPERTY(QDate, accountNotarized READ getAccountNotarized NOTIFY accountNotarizedChanged)
    W_PROPERTY(QDate, accountPaid READ getAccountPaid NOTIFY accountPaidChanged)
    W_PROPERTY(QDate, accountDecided READ getAccountDecided NOTIFY accountDecidedChanged)

private:
    QObject* qmlObject;
    netManager* mng;
    QTemporaryDir tempDir;
    QString rootPath;

    bool onboarding{false};
    bool hiring{false};

    Data::item_list<Data::document_item>* docs;
    bool documentsCompleted{false};
    void check_doc_completion();
    void uplaod_docs(int index);

    int userId{0};
    Data::People::user_item::clearances clearance{Data::People::user_item::None};

    Data::item_list<Data::People::user_item>* usrs;
    Data::item_list<Data::account_item>* acnts;
    int accountId{0};
    int accountState{0};
    QDate accountReceived{};
    QDate accountExpertized{};
    QDate accountDecided{};
    QDate accountNotarized{};
    QDate accountPaid{};

    const QString filePath(const QUrl& directory,
                           const QString& fileName) const;

    void getLastUser() noexcept;
    void getLastAccount() noexcept;
    void changePwd(const char* key, const QJsonObject& json) const;
};

}

#endif // BRIDGE_HPP
