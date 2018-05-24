//
//  YSPAdapterConfiguration.h
//  YSPExplorerSdk
//
//  Created by ZhangChao on 2017/10/16.
//  Copyright © 2017年 YunShiPei. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface YSPAdapterConfiguration : NSObject

/**
 * 设置文档预览服务器地址
 
 * 该url地址为永中预览使用的地址,如果不设置则可能出现某些需要预览的附件不能正常展示
 * 例如 : http://139.217.22.35:8080
 */
@property (nonatomic ,copy) NSString *documentPreviewAddress;

/**
 * 文档预览高速模式
 
 * 官方文档 : http://dcs.yozosoft.com/help.html#link12
 */
@property (nonatomic ,assign) BOOL documentHighSpeedMode;

/**
 * 设置在浏览器外部预览附件(默认的为NO,在sdk中以一个webview的形式打开)
 
 * 如果设置为YES,sdk会回调 - (void)documentUrlWithYongZhongServer:(NSString *)urlString; 代理方法
 */
@property (nonatomic, assign) BOOL previewDocumentOutOfSdk;

/**
 * 设置在SDK内部是否以一个页面(带返回按钮)来预览文档
 *
 */
@property (nonatomic, assign) BOOL previewDocumentWithViewController;

/**
 * 设置是否自动把文档预览服务器添加白名单（添加白名单后就不需要走拦截器了）
 *
 */
@property (nonatomic, assign) BOOL previewDocumentHostAutoAddWhiteList;

/**
 * 文档预览可支持格式 (例如：doc,docx,xls,xlsx等)

 * 如果设置该属性，仅限制预览支持的文档格式
 */
@property (nonatomic, strong) NSArray *previewDocumentFormat;

/**
 * 页面登录所需要的cookie
 
 * 系统中以cookie的形式做单点登录时使用
 * token,请求头,cookie方式只能选择一种
 */
@property (nonatomic, strong) NSHTTPCookie *cookie;

/**
 * 登录客户系统需要的url后拼接的参数
 
 * 系统中以url后拼接参数的形式做单点登录时使用
 * token,请求头,cookie方式只能选择一种
 * 接收类型 (NSDictionary)
     name : 系统名称(可以为空)
     address : 需要拼接的url
     parameter : 需要拼接的参数
 例如:
     {
         "name": "oa",
         "address": "http://ehr.elion.com.cn/psp/HR92PRD/EMPLOYEE/HRMS/c/EL_HR_MENU.EL_PT_REDIRECT.GBL",
         "parameter": "OA_Token=9935bbeea57a0f862604f9fcc2dfc40c&languageCd=ZHS"
     }
 */
@property (nonatomic, strong) NSArray *tokens;

/**
 * 登录客户系统需要在http请求头中增加的字段
 
 * 系统中以请求头的形式做单点登录时使用
 * token,请求头,cookie方式只能选择一种
 */
@property (nonatomic ,strong) NSArray *headers;

@end
