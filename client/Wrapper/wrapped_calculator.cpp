#include <iostream>

#include <QQmlEngine>
#include <QDesktopServices>
#include <QLocale>
#include <QFile>

#include "wrapped_calculator.hpp"
#include <client.hpp>
#include <Item/rent.hpp>
#include <Interface/netManager.hpp>

namespace Calculator
{
wrapped_calculator::wrapped_calculator()
    : base_wrapper<list<senior_citizen>>{}
    , exp{inner}
    , rent{new Data::rent}
{
    inner->appendItems(); // at least one senior for the calculation

    this->connect(rent,
                  &rent::calculate,
                  this,
                  &wrapped_calculator::calculate_rent);

    this->connect(rent,
                  &rent::writeToFile,
                  this,
                  &wrapped_calculator::write_to_file);

    Interface::bridge::instance().context()->setContextProperty(Data::rent::key(), rent);

    if (client_utils::is_german())
    {
        lingo = QLocale::German;
        docxName = "berechnung.docx";
    }
    else
    {
        lingo = QLocale().language();
        docxName = "calcul.docx";
    }

    docxPath += client::get_tempPath().toStdString() +'/' + docxName;

    QFile rcs(QString::fromStdString(":/data/" + docxName));

    if (!rcs.copy(QString::fromStdString(docxPath)))
        Interface::netManager::instance().replyError("Calculation Document copy error",
                                                     rcs.errorString());
    // report error throug the net manager as it is connected to the bridge
}

const std::string wrapped_calculator::sex_string(const senior_citizen::sexes& sex)
{
    if (sex == senior_citizen::M)
        if (lingo == QLocale::German)
            return "Herr";
        else
            return "M  …";
    else
        if (lingo == QLocale::German)
            return "Frau";
        else
            return "Mme…";
}

void wrapped_calculator::calculate_rent()
{
    rent->from_expectency(exp.get_expectency(rent->getBirthDay()));
}

void wrapped_calculator::write_to_file()
{
    QFile file{QString::fromStdString(docxPath)};
    if (!file.exists())
    {
            QFile rcs(QString::fromStdString(":/data/" + docxName));

            if (!rcs.copy(QString::fromStdString(docxPath)))
            Interface::netManager::instance().replyError("Calculation Document copy error",
                                                         rcs.errorString());
            // report error throug the net manager as it is connected to the bridge
    }

    file.setPermissions(QFileDevice::ReadOwner|QFileDevice::WriteOwner);

    duckx::Document doc{docxPath};
    doc.open();

    QString str{QDate::currentDate().toString("dd.MM.yyyy")};

    // skip to contract date
    auto t{doc.tables()};
    auto ro{t.rows()};
    ro.next();
    ro.next();
    ro.next();
    auto c{ro.cells()};
    auto p{c.paragraphs()};
    auto ru{p.runs()};

    ru.set_text(str.toStdString());

    p = doc.paragraphs();

    // skip to pargraphs of interests
    skip_paragraphs(p, 4);
    ru = p.runs();

    str = rent->getBirthDay().toString("dd.MM.yyyy");

    if (lingo == QLocale::German)
        ru.set_text("Geschätztes Transaktionsdatum "
                    + str.toStdString());
    else
    {
        ru.next();
        ru.set_text(str.toStdString());
    }

    end_runs(ru);
    p.next();

    int i{0};
    for (const auto& item : inner->items())
    {
        ru = p.runs();

        auto partner{inner->item_at(i)};
        const std::string sex{sex_string(partner.sex)};
        str = partner.birthDay.toString("dd.MM.yyyy");

        if (lingo == QLocale::German)
        {
            ru.set_text("Partner/in "
                        + std::to_string(i + 1)
                        + ": "
                        + sex
                        + "…………………………………………………………… "
                        + str.toStdString());
        }
        else
        {
            ru.set_text("Partenaire "
                        + std::to_string(i + 1)
                        + ": "
                        + sex
                        + "…………………………………………………………… "
                        + str.toStdString());
        }

        end_runs(ru);
        p.next();
        i++;
    }

    // erase second partner paragraph
    if (i == 1)
    {
        ru = p.runs();
        ru.set_text("");
        end_runs(ru);
        p.next();
    }

    // skip to pargraphs of interests
    skip_paragraphs(p, 3);

    ru = p.runs();

    str = QLocale().toString(rent->getmarketPrice());

    if (lingo == QLocale::German)
        str.prepend("Geschätzter Wert der Liegenschaft: 			CHF ");
    else
        skip_runs(ru, 7);

    str.append(".-");
    ru.set_text(str.toStdString());

    end_runs(ru);
    p.next();
    ru = p.runs();

    str = QLocale().toString(rent->getDab());

    if (lingo == QLocale::German)
        str.prepend("Wohnrecht: 						CHF ");
    else
        skip_runs(ru, 8);

    str.append(".-");
    ru.set_text(str.toStdString());

    end_runs(ru);
    p.next();
    ru = p.runs();

    str = QLocale().toString(rent->getBou());

    if (lingo == QLocale::German)
        str.prepend("Abschlagzahlung: 					CHF ");
    else
        skip_runs(ru, 9);

    str.append(".-");
    ru.set_text(str.toStdString());

    end_runs(ru);

    doc.save();
    file.setPermissions(QFileDevice::ReadOwner);

    if(!QDesktopServices::openUrl(QUrl::fromLocalFile(QString::fromStdString(docxPath))))
        Interface::netManager::instance().replyError("Calculation Document error",
                                                     "QDesktopervices : could not open .docx file");
}

void wrapped_calculator::end_runs(duckx::Run& run)
{
    run.next();

    for (; run.has_next(); run.next())
        run.set_text("");
}

void wrapped_calculator::skip_paragraphs(duckx::Paragraph& paragraphs, int n_skiped)
{
    for(int i{0}; i < n_skiped; i++)
        paragraphs.next();
}

void wrapped_calculator::skip_runs(duckx::Run& runs, int n_skiped)
{
    for(int i{0}; i < n_skiped; i++)
        runs.next();
}

void wrapped_calculator::print_runs(duckx::Run& runs)
{
    for (; runs.has_next(); runs.next())
        std::cout << runs.get_text() << std::endl;
}

//void wrapped_calculator::print_duckx()
//{
//    const auto lingo{QLocale().language()};
//    std::string docxName{};
//    std::string docxPath{tempPath->toStdString()};

//    if (lingo == QLocale::German)
//        docxName = "berechnung.docx";
//    else
//        docxName = "calcul.docx";

//    docxPath += '/' + docxName;

//    duckx::Document doc{docxPath};
//    doc.open();

//    using namespace std;

//    int i{0};
//    int j{0};
//    int k{0};
//    int l{0};
//    int m{0};

//    cout << "DOCUMENT TABLES" << endl;

//    for (auto t : doc.tables())
//    {
//        cout << "table :" << i << endl;
//        i++;
//        j = 0;
//        for (auto ro : t.rows())
//        {
//            cout << "__row :" << j << endl;
//            j++;
//            k = 0;
//            for (auto c : ro.cells())
//            {
//                cout << "____cell :" << k << endl;
//                k++;
//                l = 0;
//                for (auto p : c.paragraphs())
//                {
//                    cout << "______paragrap :" << l << endl;
//                    l++;
//                    m = 0;
//                    for (auto ru : p.runs())
//                    {
//                        cout << "________run :" << m << endl;
//                        m++;
//                        cout << "________" << ru.get_text() << endl;
//                    }
//                }
//            }
//        }
//    }

//    i = 0;
//    j = 0;

//    cout << "DOCUMENT PARAGRAPHES" << endl;

//    for (auto p : doc.paragraphs())
//    {
//        cout << "______paragrap :" << i << endl;
//        l++;
//        j = 0;
//        for (auto ru : p.runs())
//        {
//            cout << "________run :" << j << endl;
//            j++;
//            cout << "________" << ru.get_text() << endl;
//        }
//    }
//}
}
