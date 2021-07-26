#ifndef IQtPlugin_h__
#define IQtPlugin_h__
#include <QtWidgets/QWidget>
class IQtPlugin
{
public:
	virtual ~IQtPlugin(){}
	/**
	 * @fn       GetName
	 * @author   Crack
	 * @brief    ��ȡ�������
	 * @date     2021/4/11 22:10
	 * @param    
	 * @return   
	*/
	virtual const char* GetName() const  = 0;
	/**
	 * @fn       GetCompany
	 * @author   Crack
	 * @brief    ���������˾   
	 * @date     2021/7/26 10:49
	 * @param    
	 * @return   
	*/

	virtual const char* GetCompany() const  = 0;

	/**
	 * @fn       GetDuty
	 * @author   Crack
	 * @brief    ����Ĺ���   
	 * @date     2021/7/26 10:36
	 * @param    
	 * @return   
	*/
	virtual const char* GetDuty() const = 0;
	/**
	 * @fn       GetVersion
	 * @author   Crack
	 * @brief    ����汾��
	 * @date     2021/7/26 10:50
	 * @param    
	 * @return   
	*/
	virtual const char* GetVersion() const  = 0;
	/**
	 * @fn       GetDescription
	 * @author   Crack
	 * @brief    ���������Ϣ
	 * @date     2021/7/26 10:50
	 * @param    
	 * @return   
	*/
	virtual const char* GetDescription() const = 0;
	/**
	 * @fn       CanUnintall
	 * @author   Crack
	 * @brief    ����Ƿ����ж�� 
	 * @date     2021/7/26 10:50
	 * @param    
	 * @return   
	*/
	virtual bool CanUnintall() = 0;
	/**
	 * @fn       Initialize
	 * @author   Crack
	 * @brief    �����ʼ��
	 * @date     2021/7/26 10:51
	 * @param    
	 * @return   
	*/
	virtual bool Initialize(const char*strModulePath, const char* strDuty) = 0;
	/**
	 * @fn       Destroy
	 * @author   Crack
	 * @brief    �������
	 * @date     2021/7/26 10:51
	 * @param    
	 * @return   
	*/
	virtual bool Destroy() = 0;

	/**
	 * @fn       Widget
	 * @author   Crack
	 * @brief    ��ȡ������� 
	 * @date     2021/7/26 11:27
	 * @param    
	 * @return   
	*/
	virtual QWidget* Widget(QWidget* parent = 0) = 0;
	/**
	 * @fn       HideWidget
	 * @author   Crack
	 * @brief    Ӱ�ؽ���
	 * @date     2021/7/26 11:27
	 * @param    
	 * @return   
	*/
	virtual void HideWidget() = 0;
	/**
	 * @fn       ShowWidget
	 * @author   Crack
	 * @brief    չʾ����
	 * @date     2021/7/26 11:28
	 * @param    
	 * @return   
	*/
	virtual void ShowWidget() = 0;
};
#define IPlugin_iid "com.CrackDoc.Plugin"
QT_BEGIN_NAMESPACE
	Q_DECLARE_INTERFACE(IQtPlugin,IPlugin_iid)
QT_END_NAMESPACE
#endif // IQtPlugin_h__