//
//  YSPAdapterView.h
//  YSPExplorerSdk
//
//  Created by ZhangChao on 2017/2/20.
//  Copyright © 2017年 YunShiPei. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "YSPAdapterConfiguration.h"

// js调用开始定位的方法,由于是异步操作,需要调用evaluatingJavaScriptString方法和固定接口回传数据
#define K_JSCALLBACK_LOCATION_BEGIN @"locationBegin"
// js调用开始定位的方法,由于是异步操作,需要调用evaluatingJavaScriptString方法和固定接口回传数据
#define K_JSCALLBACK_GETLOCATION @"getLocation"
// js调用打开照相机的方法,由于是异步操作,需要调用evaluatingJavaScriptString方法和固定接口回传数据
#define K_JSCALLBACK_OPEN_CAMERA @"openCamera"
// js回调打开文档的接口,不需要回调
#define K_JSCALLBACK_OPEN_DOCUMENT @"openDocument"
// js回调关闭当前tab的接口
#define K_JSCALLBACK_CLOSE_WINDOW @"closeWindow"
// js回调传递消息的接口,不需要回调
#define K_JSCALLBACK_POST_MESSAGE_TO_NATIVE @"postMessageToNative"
// js回调下载各种资源
#define K_JSCALLBACK_DOWNLOAD_RESOURCE @"downloadResources"

typedef void (^YSPJSResponseCallback)(id responseData);

@protocol YSPAdapterDelegate <NSObject>

@optional
/**
 * 由JavaScript层调用OC方法
 * 不需要自定义交互接口的情况下,不需要实现
 
 @param interface 接口名称
 @param data JS参数数据
 @return 该方法的同步返回值
 */
- (id)javaScriptCallBackWithInterface:(NSString *)interface withData:(id)data;

/**
 * 由JavaScript层调用OC方法
 
 @param interface 接口名称
 @param data JS参数数据
 @param callback JS回调
 */
- (void)javaScriptCallBackWithInterface:(NSString *)interface
                                   data:(id)data
                               callback:(YSPJSResponseCallback)callback;

/**
 * 网页中自动登录方法,对应manager中的密码管家功能,如果manager中不带此功能,不需要实现
 
 @param urlString 用于判断该url是否可以自动登录
 @param isAdapter 是不是适配页面,常规适配页面传入YES即可,正常页面传入NO
 @return 是否可以自动登录
 */
- (BOOL)canAutoLoginWithUrlString:(NSString *)urlString isAdapterPage:(BOOL)isAdapter;

/**
 * 获取自动登录的字典

 @param currentWebUrlString 当前应用的urlString
 @return 返回指定结构的字典,字典key必须是如下指定的key,不可新增不可减少,以下字典仅为示例,对应系统的xpath获取请移步sdk文档中寻找对应方法
    eg.@{
         @"usernameTPL" : @"// *[@id=\"userId\"]",  // 用户名输入框的xpath
         @"passwordTPL" : @"// *[@id=\"userPwd\"]", // 密码输入框的xpath
         @"buttonTPL"   : @"// *[@id=\"tableheight\"]/tbody/tr[3]/td/table/tbody/tr/td[2]/img", // 登录按钮的xpath
         @"username"    : @"ming.xiao@yunshipei.com", // 有效的用户名
         @"password"    : @"1234" // 有效的密码
        }
 */
- (NSDictionary<NSString *, NSString *> *)getAutoLoginDictionary:(NSString *)currentWebUrlString;

/**
 * openWindow中的url没有匹配到的情况下会回调此方法

 @param urlString 没有匹配到的url
 @return 返回 YES 由SDK加载原网页,返回 NO 则SDK不会做任何操作,用户可以选择弹窗提示等操作,默认为 YES
 */
- (BOOL)shouldHandleNoMatcherUrlWithOpenWindow:(NSString *)urlString;

/**
 * 适配页面加载结束
 */
- (void)loadFinished;

/**
 * sdk中已经处理了金格文档预览逻辑,在没有给sdk设置服务器地址的情况下,回调该方法,在sdk外部进行上传操作

 @param parmaDict 回传的参数字典(包含处理过后的文件路径)
 */
- (void)shouldUploadDocumentWhitJinge:(NSDictionary *)parmaDict;

/**
 * 如果sdk中处理金格文档预览出现问题,可以在回调接口中添加 "debug":"true" 键值对,sdk会回调出来进行处理

 @param parmaDict 回调参数字典(js回调的原生字典)
 */
- (void)jingeDocumentWithOpenDocument:(NSDictionary *)parmaDict;

/**
 * 返回用户手机号

 @return 字典类型
 */
- (NSDictionary *)getUserInfo;

/**
 * 永中转换过后的url,直接用webview加载即可显示转换后的文档
 * 只有在configuration里的previewDocumentOutOfSdk属性设置为YES才会回调出来.

 @param urlString 永中转换过后的urlString
 */
- (void)documentUrlWithYongZhongServer:(NSString *)urlString;

/**
 * 用户自定义的loading动画，如果实现，将不运行我们SDK中的loading动画。
 *（同时也必须实现customHideLoading方法）
 @param msg 文字消息
 */
- (void)customShowLoading:(NSString *)msg;

/**
 * 用户自定义的隐藏loading动画，如果实现，将不运行我们SDK中的隐藏loading动画。
 *（同时也必须实现customShowLoading方法）
 */
- (void)customHideLoading;

@required
/**
 * 返回回调方法
 * 依赖方法
 * 此方法依赖于webviewGoBack方法,在webview不能返回的情况下,sdk会回调此方法,由用户选择需要的操作
 */
- (void)backIfNeed;

@end

@interface YSPAdapterView : UIView

/// 代理属性
@property (nonatomic, weak) id <YSPAdapterDelegate> adapterViewDelegate;

/// 要打开的urlString
@property (nonatomic, copy) NSString *defaultUrlString;

/**
 * 初始化方法
 
 @param appid 要打开的应用的applicationID
 @param delegate 代理属性
 @return 适配好的一个 view 对象
 */
- (instancetype)initWithApplicationId:(NSString *)appid delegate:(id)delegate configuration:(YSPAdapterConfiguration *)configuration;

/**
 * 初始化方法
 
 @param urlString urlString 要打开的 urlString
 @param delegate 代理属性
 @return 适配好的一个 view 对象
 */
- (instancetype)initWithUrlString:(NSString *)urlString delegate:(id)delegate;

/**
 * 初始化方法

 @param urlString urlString 要打开的 urlString
 @param delegate 代理属性
 @param configuration 配置项,如:文档预览服务器地址/cookie等属性
 @return 适配好的一个 view 对象
 */
- (instancetype)initWithUrlString:(NSString *)urlString delegate:(id)delegate configuration:(YSPAdapterConfiguration *)configuration;

/**
 * 刷新当前的webview
 */
- (void)reloadCurrentWebview;

/**
 * webview的前进方法
 * 此方法只适用于在线应用,适配应用中此方法无效.
 */
- (void)webviewGoForward;

/**
 * 当前显示的webview进行返回操作
 * 此方法必须和代理方法backIfNeed一同使用,在webview不能进行返回的情况下,sdk会回调backIfNeed方法,此处需要用户自己做返回操作.
 */
- (void)webviewGoBack;

/**
 * 用当前的tab打开一个url

 @param urlString 需要打开的urlString
 */
- (void)openWindowWithUrlString:(NSString *)urlString;

/**
 * 调用webview中的JS方法,同步方法,会阻塞JS执行

 @param javaScriptString 要调用的JS语句
 @return JS函数的返回值
 */
- (NSString *)evaluatingJavaScriptString:(NSString *)javaScriptString;

// wangping: 先注销此方法，有问题
/**
 * 通过源URL刷新webview，如果URL匹配到返回YES，否则NO
 
 @param url webview加载的源url
 @return YES or NO
 */
//-(BOOL)refreshWebView:(NSString *)url;

@end
