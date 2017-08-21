#ifndef CDRAFTPARTICIPANTLIST_H
#define CDRAFTPARTICIPANTLIST_H

#include <vector>
class CDraftParticipant;

class CDraftParticipantList : public std::vector<std::unique_ptr<CDraftParticipant>>
{
public:
    CDraftParticipantList();
    virtual ~CDraftParticipantList();
protected:
    void AddParticipant(std::unique_ptr<CDraftParticipant>);
private:
};

#endif // CDRAFTPARTICIPANTLIST_H
