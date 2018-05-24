//
//  YSPExplorerSdkManager.h
//  YSPExplorerSdk
//
//  Created by ZhangChao on 2017/2/28.
//  Copyright © 2017年 YunShiPei. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class YSPAdapterView;

typedef void (^SuccessCallback)(id response);
typedef void (^FailureCallback)(NSError *error);

@protocol YSPExplorerSdkDelegate <NSObject>

@optional
/**
 * 单点登录插入js方法

 @param host 需要处理的host
 @return 本地js脚本
 */
- (NSString *)xPathJsStringWithHost:(NSString *)host;

@required
/**
 * sdk验证错误信息

 @param message 具体错误信息,此接口回调"SDK适配能力受限"时,需要检查是否有重复用户登录,或者license登录人数上限
 */
- (void)sdkCertificateErrorWithMessage:(NSString *)message;

@end

@interface YSPExplorerSdkManager : NSObject

/// 代理变量
@property (nonatomic ,weak) id<YSPExplorerSdkDelegate> explorerSdkDelegate;

/// 服务器代理信息信息 （使用webViewProxyArray代替）
//@property (nonatomic ,strong) NSMutableDictionary<NSString *,NSString *> *configDictionary;

/* 
 * 白名单url的正则字符串数组
 * 该数组用于添加不需要sdk处理的url的正则表达式字符串,sdk中会做正则匹配,匹配到的url将不会进入sdk中的url处理系统.
 * 注意 :
       1.正则表达式是完全匹配类型,匹配到的url将不会进入sdk的url处理系统,如果该数组中添加了适配页面中的url,会导致sdk无法完成整个适配的流程.
       2.该数组中存入的正则表达式应该是请求url没有encode之前的状态.
       3.该数组需要在initSdk完成以后设置一次即可.
       4.该数组用于处理基于 NSUrlConnection 原生库发出来的请求,进入sdk的url处理系统后发生了某些错误的情况.
       5.用 NSUrlSession 以及 AFNetworking 3.0 以上的库发出的请求是不会进入sdk的url处理系统,不需要使用该数组.
       6.尽量减少该数组的使用,数组中的正则必须保证准确,否则可能出现影响适配核心功能的问题.
 */
@property (nonatomic, strong) NSMutableArray<NSString *> *whiteListUrlMatcherArray;

/** webview http 代理信息数组（每个host对应一个代理信息）**/
@property (nonatomic ,strong) NSMutableArray *webViewProxyArray;

/**
 * 单例方法

 @return 唯一实例
 */
+ (instancetype)defaultManager;

/**
 * 初始化sdk (config.metadata配置文件初始化SDK)
 * 说明:
     1.调用该方法后不可以调用 initSdkWithCompanyName:managerServer:managerProxy: 方法
     2.此接口需要 config.metadata 文件在指定工程目录
 * 需要在appdelegate中的didFinishLaunchingWithOptions:方法中调用,如果不初始化,影响整个sdk功能
 
 */
- (void)initSdk;

/**
 * 初始化sdk 单点登录用户数据(用户名)
 @param authName 用户名

 * 说明:
 * 该方法需要在调用适配方案之前调用
 */
- (void)initAuthName:(NSString *)authName;

/**
 * 设置私有DNS

 @param host host
 @param ip 对应的IP
 */
- (void)privateDnsHost:(NSString *)host ip:(NSString *)ip;

/**
 * 初始化sdk (代码方式初始化SDK，无http代理访问manager，或者有http代理但是不需要账号密码访问)
 * 说明:
    1.该方法必须传入正确的公司名称和服务器地址
    2.调用该方法后,不可以调用initSdk方法.
    3.不需要 config.metadata 文件
 
 @param companyName 公司域名
 @param managerServer 服务器地址
 @param managerProxy 服务器需要的代理地址(可为空)
 */
- (void)initSdkWithCompanyName:(NSString *)companyName
                 managerServer:(NSString *)managerServer
                  managerProxy:(NSString *)managerProxy;

/**
  * 初始化sdk （代码方式初始化SDK，有http代理访问manager，并且需要用户名密码访问代理）
  * 说明:
  1.该方法必须传入正确的公司名称和服务器地址
  2.调用该方法后,不可以调用initSdk方法.
  3.不需要 config.metadata 文件
 
  （注：目前针对代理的用户名密码只支持Basic授权）
 
  @param companyName 公司域名
  @param managerServer 服务器地址
  @param managerProxy 服务器需要的代理地址(可为空)
  @param proxyUserName 代理用户名(可为空)
  @param proxyPassword 代理密码(可为空)
*/
- (void)initSdkWithCompanyName:(NSString *)companyName
                 managerServer:(NSString *)managerServer
                  managerProxy:(NSString *)managerProxy
                 proxyUserName:(NSString *)proxyUserName
                 proxyPassword:(NSString *)proxyPassword;

/**
 * 无用户名和密码系统的登录接口(网络请求类接口)
 * 该方法在整个应用生命周期中只需要调用一次即可,成功后即可初始化 YSPAdapterView 进行适配展示
 * 集成性接口,拥有以下三个接口的所有功能,调用此接口以后不可以调用以下三个接口
        inspectCompanyInfoWithSuccess:andFailed: 接口
        loginApiWithUsername:andPassword:callbackSuccess:andFailed: 接口
        handleLoginDataWithResponseObject:callbackSuccess: 接口
 
 @param success 返回id类型 status == 1 表明检查,登录和配置成功,status == 0 则出错,会返回相应的错误信息
 @param failure NSError实例 网络请求错误回调
 */
- (void)authRequestWithSuccessCallback:(SuccessCallback)success
                             andFailed:(FailureCallback)failure;

/**
 * 无用户名和密码系统的登录接口(网络请求类接口)
 * 该方法在整个应用生命周期中只需要调用一次即可,成功后即可初始化 YSPAdapterView 进行适配展示
 * 集成性接口,拥有以下三个接口的所有功能,调用此接口以后不可以调用以下三个接口
 inspectCompanyInfoWithSuccess:andFailed: 接口
 loginApiWithUsername:andPassword:callbackSuccess:andFailed: 接口
 handleLoginDataWithResponseObject:callbackSuccess: 接口

 @param applications 所需应用id信息，id为字符串类型
 @param success 返回id类型 status == 1 表明检查,登录和配置成功,status == 0 则出错,会返回相应的错误信息
 @param failure NSError实例 网络请求错误回调
 */
- (void)authRequestWithApplicationIds:(NSArray *)applications successCallBack:(SuccessCallback)success andFailed:(FailureCallback)failure;


/**
 * 检查公司信息接口(网络请求类接口)   
 * 特有接口,在没有config.matedata的情况下使用
 
 @param success 返回id类型 status == 1 表明公司存在,status == 0 表明公司不存在
 @param failure NSError实例 网络请求错误回调
 */
- (void)inspectCompanyInfoWithSuccess:(SuccessCallback)success
                            andFailed:(FailureCallback)failure;

/**
 * 带用户名和密码的登录接口(网络请求类接口)
 * 依赖性接口,此接口依赖于inspectCompanyInfoWithSuccess:andFailed:接口,若需要调用此接口,必须在检查接口成功回调里调用
 
 @param username 用户名
 @param password 密码
 @param otherParamDict 登录接口另外需要的参数,以键值对形式传入,可为空
 @param isHandleLoginData 是否由SDK处理登录数据  YES : 由SDK处理  NO : 自己处理,可以调用handleLoginDataWithResponseObject:callbackSuccess:接口进行处理
 @param success 返回id类型 status == 1 表明登录和配置成功,status == 0 则出错,会返回相应的错误信息
 @param failure NSError实例 网络请求错误回调
 */
- (void)loginApiWithUsername:(NSString *)username
                 andPassword:(NSString *)password
        otherParamDictionary:(NSDictionary *)otherParamDict
       shouldHandleLoginData:(BOOL)isHandleLoginData
             callbackSuccess:(SuccessCallback)success
                   andFailed:(FailureCallback)failure;

/**
 * 接口数据处理方法(网络请求类接口)
 * 特有接口,在用户名密码登录接口无法满足需求的情况下,自定义Login请求,成功后调用此方法
 
 @param responseObject 登录接口返回的responseObject,类型为已经经过序列化的数据
 @param success 成功回调 status == 1 表明配置成功,status == 0 则出错,会返回相应的错误信息
 @param failure NSError实例 网络请求错误回调
 */
- (void)handleLoginDataWithResponseObject:(id)responseObject
                          callbackSuccess:(SuccessCallback)success
                                andFailed:(FailureCallback)failure;

/**
 * 上传所有日志信息
 * 日志信息可以在managerLite中进行查看
 @param userName 传入用户系统的用户名（manager统计用），没有则传nil
 @param success 成功回调 status == 1 表明上传成功,status == 0 则出错,会返回相应的错误信息
 @param failure 失败回调 NSError实例 网络请求错误回调
 */
- (void)uploadAllLogsWithUserName:(NSString *)userName Success:(SuccessCallback)success andFailed:(FailureCallback)failure;

/**
 * 更新sdk中适配包信息

 @param success 成功回调 status == 1 表明上传成功,status == 0 则出错,会返回相应的错误信息
 @param failure 失败回调 NSError实例 网络请求错误回调
 */
- (void)refreshSdkInfoWithUuid:(NSString *)uuid Success:(SuccessCallback)success andFailed:(FailureCallback)failure;

/**
 * 重置当前显示AdapterView的数据

 @param adapterView 需要重置的AdapterView实例
 */
- (void)resetAdapterInfoWithAdapterView:(YSPAdapterView *)adapterView;


/**
 * 展示水印接口

 @param isDisplay 是否展示水印
 @param isOnWindow 是否在window上展示,如果传入yes,则整个app都会共享此水印
 @param extraInfo 水印中除了时间戳和deviceId部分显示的内容
 @param showTimestamp 是否展示时间戳
 @param showDeviceId 是否展示设备ID
 @param textColor 水印内容颜色
 @param fontSize 水印文字大小
 @param textAlpha 透明度(0~1)
 */
- (void)displayWaterMark:(BOOL)isDisplay
                onWindow:(BOOL)isOnWindow
               extraInfo:(NSString *)extraInfo
           showTimestamp:(BOOL)showTimestamp
            showDeviceId:(BOOL)showDeviceId
               textColor:(UIColor *)textColor
                fontSize:(NSInteger)fontSize
               textAlpha:(CGFloat)textAlpha;

/**
 * 去除所有水印
 */
- (void)removeWaterMark;

/**
 * 添加webview http 代理信息
 
 @param host webview要访问的host（不需要http://协议前缀）
 @param proxyHostWithPort http代理的域名和端口号（不需要http://协议前缀）
 @param proxyUserName http代理的用户名，没有传nil即可（目前只支持basic协议的用户名密码访问）
 @param proxyPassword http代理的密码，没有传nil即可
 */
- (void)addWebViewHttpProxy:(NSString *)host
          proxyHostWithPort:(NSString *)proxyHostWithPort
               proxyUserName:(NSString *)proxyUserName
               proxyPassword:(NSString *)proxyPassword;

/**
 * 重置sdk中所有数据
 */
+ (void)resetSdk;

@end
