#pragma once

#ifndef _NAC_TIMEOUT_H_
#define _NAC_TIMEOUT_H_

#include <functional>

typedef std::function<void()> stdBind_Func;

class NAC_Timeout
{
public:
	NAC_Timeout();
	virtual ~NAC_Timeout();

	bool SetTimer( __in stdBind_Func _Func, __in int _nTime_Sec );

private:
	stdBind_Func m_Func;
};

#endif /* NC_TIMEOUT_H_ */
