#ifndef QtPluginModule_h__
#define QtPluginModule_h__

#include <QObject>
#include "IQtPluginModule.h"
#include "PublicMarco.h"
#include <QMutex>

class CQtPluginModule : public QObject,public IQtPluginModule
{
    Q_OBJECT

public:
    CQtPluginModule(QObject *parent = Q_NULLPTR);
	~CQtPluginModule();
public:
	virtual std::string GetName() const;

	virtual std::string GetCompany() const;

	virtual std::string GetVersion() const;

	virtual std::string GetDescription() const;
	
	virtual bool Initialise();
	
	virtual bool Uninitialise();
	
public:
	/**
	 * @fn  LoadPlugin
	 * @brief 通过插件名,插件路径,插件图标等信息,加载插件
	 * @param const std::string & strName:		插件名
	 * @param const std::string & strDllPath:	插件路径
	 * @param const std::string & strIconPath:	图标路径
	 * @param const std::string & strDuty:		职责信息,默认为system,[空或者'system'均认为是隐含职责system]
	 * @return bool 返回是否成功
	 */
	virtual bool LoadPlugin(const char* strName,
		const char* strDllPath,
		const char* strIconPath = "",
		const char* strDuty = "system");

	/**
	 * @fn  LoadPluginsFromFile
	 * @brief 从配置文件加载插件
	 * @param const std::string & strPath: 配置文件路径
	 * @return bool 返回是否成功
	 */
	bool LoadPluginsFromFile(const char* strPath, const char* strDuty);

	/**
	 * @fn  UnloadPlugin  [废弃]
	 * @brief   通过插件名(该名字来自IPlugin::GetName())卸载插件
	 * @param const std::string & strName: 插件名
	 * @param bool bForceUnload: 强制卸载
	 * @return bool
	 */
	bool UnloadPlugin(const char* strName, bool bForceUnload = false);

	/**
	 * @fn  UnloadPlugin
	 * @brief   通过插件,卸载插件
	 * @param IQtPlugin * pPlugin: 插件指针
	 * @param bool bForceUnload: 强制卸载
	 * @return bool
	 */
	bool UnloadPlugin(IQtPlugin* pPlugin, bool bForceUnload = false);

	/**
	 * @fn  UnloadPluginByDllPath
	 * @brief   通过插件路劲卸载插件(完整匹配路径)
	 * @param const std::string & strDllPath: 插件完整路径,必须和插件配置文件中一致
	 * @param bool bForceUnload: 强制卸载
	 * @return bool 返回是否成功
	 */
	bool UnloadPluginByDllPath(const char* strDllPath, bool bForceUnload = false);

	/**
	 * @fn  UnloadPluginsFromFile
	 * @brief 通过配置文件卸载插件
	 * @param const std::string & strPath: 配置文件路径
	 * @param bool bForceUnload: 强制卸载
	 * @return void
	 */
	void UnloadPluginsFromFile(const char* strPath, bool bForceUnload = false);

	/**
	 * @fn  UnloadPluginsByDuty
	 * @brief 按照插件职责卸载插件
	 * @param const std::string & strDuty: 职责
	 * @param bool bForceUnload: 强制卸载
	 * @return void
	 */
	bool UnloadPluginsByDuty(const char* strDuty, bool bForceUnload = false);

	/**
	 * @fn  UnloadAllPlugins
	 * @brief 卸载所有插件
	 * @return void
	 */
	void UnloadAllPlugins();

	/**
	 * @fn  RegisterObserver
	 * @brief 注册插件模块的观察者,可以处理插件加载和卸载的事件通知
	 * @param IQtPluginModuleObserver * pObserver: 插件模块观察者
	 * @return bool 返回是否注册成功
	 */
	bool RegisterObserver(IQtPluginModuleObserver* pObserver);

	/**
	 * @fn  UnregisterObserver
	 * @brief 取消插件模块的观察者注册,
	 * @param IQtPluginModuleObserver * pObserver: 需要取消注册的观察者
	 * @return void
	 */
	void UnregisterObserver(IQtPluginModuleObserver* pObserver);

	/**
	 * @fn  Travel
	 * @brief 遍历所有插件
	 * @param IQtPluginInfoVisitor & rVisitor:
	 * @return void
	 */
	void Travel(IQtPluginInfoVisitor& rVisitor);

	/**
	 * @fn  IsPluginLoaded
	 * @brief 返回某个插件是否加载
	 * @param const std::string& strPluginName:
	 * @return bool
	 */
	bool IsPluginLoaded(const char* strPluginName);

protected:
	/**
	 * @fn  NotifyPluginLoad
	 * @brief 通知插件注册
	 * @param const std::string & strPluginPath:
	 * @param IQtPlugin * pPlugin:
	 * @return void
	 */
	void NotifyPluginLoad(const char* strModulePath, const char* strPluginPath, IQtPlugin* pPlugin, const char* strDuty);

	/**
	 * @fn  NotifyPluginUnload
	 * @brief 通知插件卸载
	 * @param IQtPlugin * pPlugin:
	 * @return void
	 */
	void NotifyPluginUnload(IQtPlugin* pPlugin, const char* strDuty);

protected:
	QMutex m_PluginsInfoLock;//插件信息锁
	void* m_mapPluginInfos;
	//QMap<QString, IQtPlugin*> m_mapPluginInfos;	//插件信息容器

	QMutex m_ObserverLock;//插件模块观察者锁
	void* m_lstObserver;
	//QList<IQtPluginModuleObserver*> m_lstObserver;			//插件模块观察者容器
};
DECLARE_ACMODULE(QtPlugin, extern)
#endif // QtPluginModule_h__