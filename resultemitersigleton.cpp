#include "resultemitersigleton.h"

// static singleton instance
ResultEmiterSigleton *ResultEmiterSigleton::instance = NULL;


// static get instance
ResultEmiterSigleton* ResultEmiterSigleton::getInstance()
{
	if( instance == NULL ){
		instance = new ResultEmiterSigleton;
	}
	return instance;
}

// sent result request
void ResultEmiterSigleton::sentResultRequest(const QStringList &list) const {
	emit sentResult( list );
}
