#ifndef S_DOCUMENT_HPP
#define S_DOCUMENT_HPP

#include <server.hpp>

#include "s_account.hpp"
#include "s_list.hpp"
#include <server_utils.hpp>
#include <Item/document_item.hpp>

namespace Data
{
struct s_document final : public document_item
        , public s_base_data
{
    s_document();

    void set(const nanodbc::result& res) override;

    const std::string insert(const People::s_user& usr, const s_account* acnt = nullptr) const;
    const std::string update(const People::s_user& usr, const s_account* acnt = nullptr) const;
    const std::string remove(const People::s_user& usr, const s_account* acnt = nullptr) const;

    static void foreign_update(std::string& query,
                               bool complete,
                               const s_account* acnt = nullptr);

    static void condition(std::string& query,
                          const People::s_user& usr,
                          const s_account* acnt = nullptr);

    static void update_reply(nanodbc::result& res,
                             Json::Value& json,
                             const s_account* acnt = nullptr);

    static const constexpr std::string select(const People::s_user& usr,
                                              const s_account* acnt)
    {
        return "SELECT DISTINCT "
               "d.Id, "
               "Category, "
               "Extension, "
               "FileName, "
               "isUploaded "
               "FROM Account a, "
               "Document d, "
               "[User] u "
               "WHERE d.AccountId = "
                + std::to_string(acnt->id) +
                " "
                + server::utils::clearance_close(usr);
    }

    void set_directory(int acnt_id);
    const filesystem::path get_path(int acnt_id = 0) const;

private:
    const filesystem::path get_directory(int acnt_id) const;
};

template<>
inline bool item_list<s_document>::is_completed() const
{
    if (m_items.empty()) return false;

    // retrieve isPPE value from the fist document's AccountId
    auto result{nanodbc::execute(server::server::get().connection,
                                 "SELECT DISTINCT a.isPPE, a.Id "
                                 "FROM Account a, Document d "
                                 "WHERE d.id = "
                                 + std::to_string(m_items[0].id) +
                                 " AND a.Id = d.AccountId")};
    result.next();

    if (!document_item::documents_completed<s_document>(m_items,
                                                        result.get<int>("isPPE")))
        return false;

    // double check all files are corectly updated
    for (const auto& doc : m_items)
    {
        if (!filesystem::exists(doc.get_path(result.get<int>("Id"))))
            return false;
    }

    return true;
}

template<>
template<>
inline void s_list<s_document>::foreign_update(std::string& query,
                                               bool complete,
                                               const s_account* acnt)
{
    std::string str{server::utils::update_flag(
                    account_item::DocumentsCompleted,
                    "State",
                    complete)};
    acnt->foreign_update(str, complete, acnt);
    query.append(str);
}
}

#endif // s_document_HPP
