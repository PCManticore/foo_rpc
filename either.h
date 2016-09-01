#pragma once

#define SENTINEL_ERROR -9999

template<class T>
class Maybe {
private:
	T obj;
	DWORD errorCode = SENTINEL_ERROR;

	Maybe(DWORD t) : errorCode(t) {}

public:

	static Maybe<T> withError(DWORD code) {
		return Maybe<T>(code);
	};

	Maybe(T t) : obj(t) {}

	DWORD isFailed() {
		return this->errorCode != SENTINEL_ERROR;		
	}

	DWORD error() {
		return this->errorCode;
	}

	T result() {
		return this->obj;
	}
};


