#include <iostream>
#include <forward_list>
#include <map>

class IObserver {
public:
	virtual ~IObserver(){}
	virtual void onNotify() = 0;
};

class ISubject {
public:
	virtual ~ISubject() {}

	virtual void addObserver(int message, IObserver* observer) {
		auto it = m_observer.find(message);

		if (it == m_observer.end()) {
			m_observer[message] = observerList_t();
		}

		m_observer[message].push_front(observer);
	}
	void removeObserver(int message, IObserver* observer) {
		auto it = m_observer.find(message);

		if (it == m_observer.end()) {
			observerList_t& list = m_observer[message];
			for (observerList_t::iterator iter = list.begin(); iter != list.end();)
			{
				if ((*iter) == observer) {
					list.remove(observer);
				}
				else {
					++iter;
				}
			}
		}

		m_observer[message].remove(observer);
	}
	void notify() {
		for (observerMap_t::iterator it = m_observer.begin(); it != m_observer.end(); ++it) {
			for (auto& o : m_observer[it->first]) {
				o->onNotify();
			}
		}
	}

private:
	typedef std::forward_list<IObserver*> observerList_t;
	typedef std::map<int, observerList_t> observerMap_t;
	observerMap_t m_observer;
};


class Watcher : public IObserver {
public:
	explicit Watcher(ISubject& subject, int message, const std::string& name) : m_subject(subject), m_message(message), m_name(name) {
		m_subject.addObserver(m_message, this);
	}
	~Watcher() {
		m_subject.removeObserver(m_message, this);
	}
	void onNotify() override {
		std::cout << "Say hello " << m_name << std::endl;
	}

private:
	std::string m_name;
	ISubject& m_subject;
	int m_message;
};

class SomeSubject : public ISubject {
public:
	enum MessageTypes {
		PLAYSOUNDS,
		HANDLEPHYSICS,
		LOG
	};

};

int main() {
	SomeSubject subject;

	Watcher watcher1(subject, SomeSubject::PLAYSOUNDS, "watcher1");
	Watcher watcher2(subject, SomeSubject::PLAYSOUNDS, "watcher2");
	Watcher watcher3(subject, SomeSubject::PLAYSOUNDS, "watcher3");

	subject.notify();
	
}