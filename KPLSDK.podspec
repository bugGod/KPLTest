Pod::Spec.new do |s|
  s.name = 'KPLSDK'
  s.version = '0.0.2'
  s.summary = 'kpl sdk  test'
  s.description = 'kpl is gool sdk,nice'
  s.homepage = 'http://www.baidu.com'
  s.license = 'MIT'
  s.authors = {'Answer' => 'super_ai_bsk@163.com'}
  s.platform = :ios, '8.0'
  s.source = {:git => 'https://github.com/bugGod/KPLTest.git', :tag => s.version}
  s.vendored_frameworks = 'KPLSDK/testFramework.framework'
  s.resources = 'KPLSDK/YSPSdk.bundle'
end
