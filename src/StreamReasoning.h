#ifndef STREAMREASONING_H
#define	STREAMREASONING_H

#include "ClauseListener.h"
#include "WaspFacade.h"

#include <unordered_set>
using namespace std;

class StreamReasoning : public ClauseListener
{
    public:
        StreamReasoning(WaspFacade& w) : waspFacade(w) { waspFacade.attachClauseListener(this); }
        void onLearningClause(Clause* clause) { clauses.insert(clause); }
        void onDeletingClause(Clause* clause) { clauses.erase(clause); }
        void solve();
        
    private:
        WaspFacade& waspFacade;
        unordered_set<Clause*> clauses;
};

#endif