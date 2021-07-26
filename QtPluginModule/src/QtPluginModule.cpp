#include "QtPluginModule.h"
#include "IQtPlugin.h"
#include <QMap>
#include <QList>
#include <QPluginLoader>
#include <QDir>
#include <QFileInfo>
#include <assert.h>

CQtPluginModule::CQtPluginModule(QObject* parent)
	:m_mapPluginInfos(new QMap<QString, IQtPlugin*>())
	, m_lstObserver(new QList<IQtPluginModuleObserver*>())
{

}

CQtPluginModule::~CQtPluginModule(void)
{
	// 析构函数再释放资源
}
std::string CQtPluginModule::GetName() const
{
	return "";
}

std::string CQtPluginModule::GetCompany() const
{
	return "";
}

std::string CQtPluginModule::GetVersion() const
{
	return "";
}

std::string CQtPluginModule::GetDescription() const
{
	return "";
}

bool CQtPluginModule::Initialise()
{
	return true;
}

bool CQtPluginModule::Uninitialise()
{
	return true;
}
bool CQtPluginModule::LoadPlugin(const char* strName, const char* strDllPath, const char* strIconPath /*= ""*/, const char* strDuty /*= "system"*/)
{
	QString qstrDllPath = QString::fromLocal8Bit(strDllPath);

	QPluginLoader pluginLoader(qstrDllPath);

	QObject* loadPlugin = pluginLoader.instance();
	if (loadPlugin)
	{
		IQtPlugin* pPlugin = qobject_cast<IQtPlugin*>(loadPlugin);
		if (pPlugin)
		{
			m_PluginsInfoLock.lock();

			QMap<QString, IQtPlugin*>& mapPluginInfos = *static_cast<QMap<QString, IQtPlugin*>*>(m_mapPluginInfos);

			QMap<QString, IQtPlugin*>::iterator it = mapPluginInfos.find(strName);
			if (it != mapPluginInfos.end())
			{
				m_PluginsInfoLock.unlock();
				return false;
			}
			mapPluginInfos[QString::fromLocal8Bit(strName)] = pPlugin;
			m_PluginsInfoLock.unlock();
			return true;
		}
	}
	return false;
}
bool CQtPluginModule::LoadPluginsFromFile(const char* strPath, const char* strDuty)
{
	QDir pluginsDir(QString::fromLocal8Bit(strPath));
	foreach(QString filename, pluginsDir.entryList(QDir::Files))
	{
		QFileInfo info(filename);
		if (info.suffix() == ".dll" || info.suffix() == ".so")
		{
			bool bSucc = LoadPlugin(info.baseName().toLocal8Bit(), info.absoluteFilePath().toLocal8Bit(), "", strDuty);
			// 断言成功
			assert(bSucc);
		}
	}
	return false;
}

bool CQtPluginModule::UnloadPlugin(const char* strName, bool bForceUnload /*= false*/)
{
	m_PluginsInfoLock.lock();

	QMap<QString, IQtPlugin*>& mapPluginInfos = *static_cast<QMap<QString, IQtPlugin*>*>(m_mapPluginInfos);

	QMap<QString, IQtPlugin*>::iterator it = mapPluginInfos.find(strName);

	if (it != mapPluginInfos.end())
	{
		IQtPlugin* pIQtPlugin = it.value();
		if (pIQtPlugin == nullptr)
		{
			m_PluginsInfoLock.unlock();
			return false;
		}
		if (bForceUnload || (!bForceUnload && pIQtPlugin->CanUnintall()))
		{
			// 卸载插件
			delete pIQtPlugin;
			mapPluginInfos.erase(it);
			pIQtPlugin = nullptr;
			m_PluginsInfoLock.unlock();
			return true;
		}
	}
	m_PluginsInfoLock.unlock();
	return false;
}

bool CQtPluginModule::UnloadPlugin(IQtPlugin* pPlugin, bool bForceUnload /*= false*/)
{
	return UnloadPlugin(pPlugin->GetName(), bForceUnload);
}

bool CQtPluginModule::UnloadPluginByDllPath(const char* strDllPath, bool bForceUnload /*= false*/)
{
	QFileInfo info(strDllPath);
	bool bSucc = UnloadPlugin(info.baseName().toLocal8Bit(), bForceUnload);
	
	return bSucc;

}
void CQtPluginModule::UnloadPluginsFromFile(const char* strPath, bool bForceUnload /*= false*/)
{
	QDir pluginsDir(QString::fromLocal8Bit(strPath));
	foreach(QString filename, pluginsDir.entryList(QDir::Files))
	{
		QFileInfo info(filename);
		if (info.suffix() == ".dll" || info.suffix() == ".so")
		{
			bool bSucc = UnloadPlugin(info.baseName().toLocal8Bit(), bForceUnload);
			// 断言成功
			assert(bSucc);
		}
	}
}

bool CQtPluginModule::UnloadPluginsByDuty(const char* strDuty, bool bForceUnload /*= false*/)
{
	QMap<QString, IQtPlugin*>& mapPluginInfos = *static_cast<QMap<QString, IQtPlugin*>*>(m_mapPluginInfos);

	QMap<QString, IQtPlugin*>::iterator it = mapPluginInfos.begin();

	bool IsExsit = false;
	for (; it != mapPluginInfos.end(); ++it)
	{
		IQtPlugin* pIPlugin = it.value();
		if (pIPlugin != nullptr && strcmp(pIPlugin->GetDuty(), strDuty) == 0)
		{
			std::string strName = pIPlugin->GetName();
			IsExsit = UnloadPlugin(strName.c_str(), bForceUnload);
		}
	}
	return IsExsit;
}

void CQtPluginModule::UnloadAllPlugins()
{
	QMap<QString, IQtPlugin*>& mapPluginInfos = *static_cast<QMap<QString, IQtPlugin*>*>(m_mapPluginInfos);

	QMap<QString, IQtPlugin*>::iterator it = mapPluginInfos.begin();

	for (; it != mapPluginInfos.end(); ++it)
	{
		IQtPlugin* pIPlugin = it.value();
		if (pIPlugin == nullptr)
		{
			continue;
		}
		std::string strName = pIPlugin->GetName();

		UnloadPlugin(strName.c_str(), true);
	}
}

bool CQtPluginModule::RegisterObserver(IQtPluginModuleObserver* pObserver)
{
	m_ObserverLock.lock();
	if (pObserver != NULL)
	{
		QList<IQtPluginModuleObserver*>& lstObservers = *static_cast<QList<IQtPluginModuleObserver*>*>(m_lstObserver);
		lstObservers.push_back(pObserver);

		m_ObserverLock.unlock();
		return true;
	}
	m_ObserverLock.unlock();
	return false;
}

void CQtPluginModule::UnregisterObserver(IQtPluginModuleObserver* pObserver)
{
	m_ObserverLock.unlock();
	QList<IQtPluginModuleObserver*>& lstObservers = *static_cast<QList<IQtPluginModuleObserver*>*>(m_lstObserver);

	QList<IQtPluginModuleObserver*>::iterator it0 = lstObservers.begin();

	for (; it0 != lstObservers.end(); ++it0)
	{
		IQtPluginModuleObserver* observer = *it0;
		if (observer == pObserver)
		{
			lstObservers.erase(it0);
			if (observer != NULL)
			{
				delete observer;
				observer = NULL;
			}
			break;
		}
	}
	m_ObserverLock.unlock();
}

void CQtPluginModule::Travel(IQtPluginInfoVisitor& rVisitor)
{
	QMap<QString, IQtPlugin*>& mapPluginInfos = *static_cast<QMap<QString, IQtPlugin*>*>(m_mapPluginInfos);

	QMap<QString, IQtPlugin*>::iterator it = mapPluginInfos.begin();

	for (; it != mapPluginInfos.end(); ++it)
	{
		IQtPlugin* pPlugin = it.value();

		std::string strName = it.key().toLocal8Bit().data();

		rVisitor.Visit(strName.c_str(), pPlugin);
	}
}

bool CQtPluginModule::IsPluginLoaded(const char* strPluginName)
{
	QMap<QString,IQtPlugin*>& mapPluginInfos = *static_cast<QMap<QString, IQtPlugin*>*>(m_mapPluginInfos);

	QMap<QString,IQtPlugin*>::iterator it = mapPluginInfos.find(strPluginName);

	if (it != mapPluginInfos.end())
	{
		return false;
	}
	return true;
}

void CQtPluginModule::NotifyPluginLoad(const char* strModulePath, const char* strPluginPath, IQtPlugin* pPlugin, const char* strDuty)
{
	m_ObserverLock.lock();
	QList<IQtPluginModuleObserver*>& lstObservers = *static_cast<QList<IQtPluginModuleObserver*>*>(m_lstObserver);

	QList<IQtPluginModuleObserver*>::iterator it0 = lstObservers.begin();

	for (; it0 != lstObservers.end(); ++it0)
	{
		IQtPluginModuleObserver* observer = *it0;
		if (observer != NULL)
		{
			observer->OnPluginLoad(strModulePath, strPluginPath, pPlugin, strDuty);
		}
	}
	m_ObserverLock.unlock();
}

void CQtPluginModule::NotifyPluginUnload(IQtPlugin* pPlugin, const char* strDuty)
{
	m_ObserverLock.lock();
	QList<IQtPluginModuleObserver*>& lstObservers = *static_cast<QList<IQtPluginModuleObserver*>*>(m_lstObserver);

	QList<IQtPluginModuleObserver*>::iterator it0 = lstObservers.begin();

	for (; it0 != lstObservers.end(); ++it0)
	{
		IQtPluginModuleObserver* observer = *it0;
		if (observer != NULL)
		{
			observer->OnPluginUnload(pPlugin, strDuty);
		}
	}
	m_ObserverLock.unlock();
}
IMPLEMENT_ACMODULE(QtPlugin)