#ifndef BASE_DATA_HPP
#define BASE_DATA_HPP

#include <QObject>
#include <wobjectdefs.h>

namespace Data
{
class base_data : public QObject
{
    W_OBJECT(base_data)

public:
    virtual void clear() {}

    void validate(int id)
    W_SIGNAL(validate, id)

    void loadFrom(int id)
    W_SIGNAL(loadFrom, id)

    void loaded()
    W_SIGNAL(loaded)

    bool getCompleted() const;
    void setCompleted(bool newCompleted);

    void completedChanged()
    W_SIGNAL(completedChanged)

    W_PROPERTY(bool, completed READ getCompleted NOTIFY completedChanged)

protected:
    explicit base_data(QObject* parent = nullptr);

    bool completed;
};

}

#endif // BASE_DATA_HPP
