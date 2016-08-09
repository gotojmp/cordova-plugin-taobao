package com.gotojmp.cordova.taobao;

import android.widget.Toast;

import com.alibaba.sdk.android.AlibabaSDK;
import com.alibaba.sdk.android.callback.InitResultCallback;
import com.alibaba.sdk.android.push.CloudPushService;
import com.alibaba.sdk.android.push.CommonCallback;

import com.alibaba.sdk.android.openaccount.OauthService;
import com.alibaba.sdk.android.oauth.OauthPlateform;
import com.alibaba.sdk.android.openaccount.ui.OpenAccountUIService;
import com.alibaba.sdk.android.openaccount.model.OpenAccountSession;
import com.alibaba.sdk.android.openaccount.callback.LoginCallback;

import com.umeng.socialize.ShareAction;
import com.umeng.socialize.UMShareListener;
import com.umeng.socialize.bean.SHARE_MEDIA;
import com.umeng.socialize.media.UMImage;

import com.alibaba.sdk.android.media.MediaService;
import com.alibaba.sdk.android.media.upload.UploadListener;
import com.alibaba.sdk.android.media.upload.UploadOptions;
import com.alibaba.sdk.android.media.upload.UploadTask;
import com.alibaba.sdk.android.media.utils.FailReason;
import java.io.File;

//import com.alibaba.sdk.android.login.LoginService;
//import com.alibaba.sdk.android.login.callback.LogoutCallback;
//import com.alibaba.sdk.android.login.callback.LoginCallback;

import com.alibaba.sdk.android.trade.TradeConfigs;
import com.alibaba.sdk.android.trade.TradeConstants;
import com.alibaba.sdk.android.trade.TradeService;
import com.alibaba.sdk.android.trade.ItemService;
import com.alibaba.sdk.android.trade.CartService;

import com.alibaba.sdk.android.trade.model.TaokeParams;
import com.alibaba.sdk.android.trade.model.TradeResult;

import com.alibaba.sdk.android.trade.page.Page;
import com.alibaba.sdk.android.trade.page.ItemDetailPage;
import com.alibaba.sdk.android.trade.page.MyCartsPage;
import com.alibaba.sdk.android.trade.page.MyOrdersPage;
import com.alibaba.sdk.android.trade.callback.TradeProcessCallback;

import com.alibaba.sdk.android.session.model.User;
import com.alibaba.sdk.android.session.model.Session;
import com.alibaba.sdk.android.session.SessionListener;

import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.CallbackContext;
import org.apache.cordova.LOG;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class Taobao extends CordovaPlugin {

        private static final String TAG = "Cordova.Plugin.Taobao";
        private static final String PIDKEY = "TAOKEPID";
        private static final String UMENGAPPKEY = "umeng_appkey_android";
        private static final String WXAPPID = "wx_appid";
        private static final String WXAPPSECRET = "wx_appsecret";
        private static final String WXAPPURL = "wx_appurl";
        private static final String QQAPPID = "qq_appid";
        private static final String QQAPPKEY = "qq_appkey";
        private static final String QQAPPURL = "qq_appurl";
        private static final String WBAPPKEY = "wb_appkey";
        private static final String WBAPPSECRET = "wb_appsecret";
        private static final String WBCALLBACKURL = "wb_callbackurl";

        private static Taobao instance;

        private static CallbackContext currentCallbackContext;

        private String TAOKEPID;

        public Taobao() {
                instance = this;
        }

        @Override
        protected void pluginInitialize() {
                super.pluginInitialize();

                TAOKEPID = preferences.getString(PIDKEY, "");
                TradeConfigs.defaultTaokePid = TAOKEPID;
                TradeConfigs.defaultItemDetailWebViewType = TradeConstants.BAICHUAN_H5_VIEW;
                TradeConfigs.defaultISVCode = "up-app";

                //AlibabaSDK.turnOnDebug();
                AlibabaSDK.asyncInit(cordova.getActivity(), new InitResultCallback() {
                        @Override
                        public void onSuccess() {
                                //LOG.d(TAG, "初始化成功");
                                //Toast.makeText(cordova.getActivity(), "初始化成功", Toast.LENGTH_SHORT).show();
                                initCloudChannel();
                                setSSOAppKey();
                                MediaService.enableHttpDNS(); //果用户为了避免域名劫持，可以启用HttpDNS
                        }
                        @Override
                        public void onFailure(int code, String msg) {
                                //LOG.d(TAG, "初始化异常"+code+msg);
                                //Toast.makeText(cordova.getActivity(), "初始化异常"+code+msg, Toast.LENGTH_SHORT).show();
                        }
                });
        }

        @Override
        public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {
                LOG.d(TAG, action + " is called.");

                if (action.equals("useNativeTaobao")) {
                        return useNativeTaobao(args, callbackContext);
                } else if (action.equals("showItemDetailByItemId")) {
                        return showItemDetailByItemId(args, callbackContext);
                } else if (action.equals("showTaoKeItemDetailByItemId")) {
                        return showTaoKeItemDetailByItemId(args, callbackContext);
                } else if (action.equals("showCart")) {
                        return showCart(callbackContext);
                } else if (action.equals("showOrder")) {
                        return showOrder(callbackContext);
                } else if (action.equals("showPage")) {
                        return showPage(args, callbackContext);
                } else if (action.equals("openLoginBox")) {
                        return openLoginBox(callbackContext);
                } else if (action.equals("bindAccount")) {
                        return bindAccount(args, callbackContext);
                } else if (action.equals("unbindAccount")) {
                        return unbindAccount(args, callbackContext);
                } else if (action.equals("shareTo")) {
                        return shareTo(args, callbackContext);
                } else if (action.equals("uploadImage")) {
                        return uploadImage(args, callbackContext);
                }
                return super.execute(action, args, callbackContext);
        }

        private void setSSOAppKey() {
                String uMengAppKey = preferences.getString(UMENGAPPKEY, "");
                String wxAppId = preferences.getString(WXAPPID, "");
                String wxAppSecret = preferences.getString(WXAPPSECRET, "");
                String wxAppUrl = preferences.getString(WXAPPURL, "");
                String qqAppId = preferences.getString(QQAPPID, "");
                String qqAppKey = preferences.getString(QQAPPKEY, "");
                String qqAppUrl = preferences.getString(QQAPPURL, "");
                String wbAppKey = preferences.getString(WBAPPKEY, "");
                String wbAppSecret = preferences.getString(WBAPPSECRET, "");
                String wbCallbackUrl = preferences.getString(WBCALLBACKURL, "");

                OauthService oauthService = AlibabaSDK.getService(OauthService.class);
                oauthService.addAppCredential(OauthPlateform.WEIXIN, wxAppId, wxAppSecret);
                oauthService.addAppCredential(OauthPlateform.QQ, qqAppId, qqAppKey);
                oauthService.addAppCredential(OauthPlateform.WEIBO, wbAppKey, wbAppSecret);
        }

        protected boolean shareTo(JSONArray args, CallbackContext callbackContext) {
                try {
                        final int platform = args.getInt(0);//0-微信朋友圈，1-微信好友，2-新浪微博，3-QQ空间，4-QQ好友
                        if (platform < 0 || platform > 4) {
                                callbackContext.error("get arg error");
                                return true;
                        }
                        final String text = args.getString(1);
                        final String imgUrl = args.getString(2);
                        final String title = args.getString(3);
                        final String url = args.getString(4);
                        final int sendMode = args.getInt(5);//0-通常发送，1-静默发送
                        cordova.getActivity().runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                        UMImage image = new UMImage(cordova.getActivity(), imgUrl);
                                        SHARE_MEDIA shareMedia = SHARE_MEDIA.SMS;
                                        switch (platform) {
                                                case 0:
                                                        shareMedia = SHARE_MEDIA.WEIXIN_CIRCLE;
                                                        break;
                                                case 1:
                                                        shareMedia = SHARE_MEDIA.WEIXIN;
                                                        break;
                                                case 2:
                                                        shareMedia = SHARE_MEDIA.SINA;
                                                        break;
                                                case 3:
                                                        shareMedia = SHARE_MEDIA.QZONE;
                                                        break;
                                                case 4:
                                                        shareMedia = SHARE_MEDIA.QQ;
                                                        break;
                                        }
                                        new ShareAction(cordova.getActivity())
                                                .setPlatform(shareMedia)
                                                .setCallback(new UMShareListener() {
                                                        @Override
                                                        public void onResult(SHARE_MEDIA platform) {
                                                                //Toast.makeText(cordova.getActivity(), platform + " 分享成功啦", Toast.LENGTH_SHORT).show();
                                                                currentCallbackContext.success();
                                                        }
                                                        @Override
                                                        public void onError(SHARE_MEDIA platform, Throwable t) {
                                                                //Toast.makeText(cordova.getActivity(), platform + " 分享失败啦", Toast.LENGTH_SHORT).show();
                                                                currentCallbackContext.error("error");
                                                        }
                                                        @Override
                                                        public void onCancel(SHARE_MEDIA platform) {
                                                                //Toast.makeText(cordova.getActivity(), platform + " 分享取消了", Toast.LENGTH_SHORT).show();
                                                                currentCallbackContext.error("cancel");
                                                        }
                                                })
                                                .withTitle(title)
                                                .withText(text)
                                                .withTargetUrl(url)
                                                .withMedia(image)
                                                .share();
                                }
                        });
                } catch (Exception e) {
                        LOG.e(TAG, "get arg error");
                        callbackContext.error("get arg error");
                        return true;
                }

                currentCallbackContext = callbackContext;
                return true;
        }

        protected boolean uploadImage(JSONArray args, CallbackContext callbackContext) {
                try {
                        final String mediaNameSpace = preferences.getString("taobao_media_ns", "");
                        final String filePath = args.getString(0);
                        final String dir = args.getString(1);
                        final String fileName = args.getString(2);
                        //final int imgType = args.getInt(3);
                        final CallbackContext _callbackContext = callbackContext;
                        cordova.getActivity().runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                        MediaService mediaService = AlibabaSDK.getService(MediaService.class);
                                        mediaService.enableLog(); //在调试时，可以打印日志。正式上线前可以关闭
                                        UploadListener listener = new UploadListener() {
                                                @Override
                                                public void onUploading(UploadTask uploadTask) {
                                                        LOG.e(TAG, "---上传中---已上传大小："+uploadTask.getCurrent()
                                                                +" 总文件大小："+uploadTask.getTotal());
                                                }
                                                @Override
                                                public void onUploadFailed(UploadTask uploadTask, FailReason failReason) {
                                                        LOG.e(TAG, "---上传失败---");
                                                        _callbackContext.error("fail");
                                                }
                                                @Override
                                                public void onUploadComplete(UploadTask uploadTask) {
                                                        LOG.e(TAG, "---上传成功---");
                                                        JSONObject info = new JSONObject();
                                                        try {
                                                                info.put("url", uploadTask.getResult().url);
                                                        } catch (JSONException e) {
                                                                LOG.e(TAG, "make json error");
                                                        }
                                                        _callbackContext.success(info);
                                                }
                                                @Override
                                                public void onUploadCancelled(UploadTask uploadTask) {
                                                        LOG.e(TAG, "---上传取消---");
                                                        _callbackContext.error("cancel");
                                                }
                                        };
                                        UploadOptions options = new UploadOptions.Builder().dir(dir).aliases(fileName).build();
                                        String taskId = mediaService.upload(new File(filePath), mediaNameSpace, options, listener);
                                        LOG.e(TAG, taskId);
                                        //mediaService.pauseUpload(taskId);
                                        //mediaService.resumeUpload(taskId, listener);
                                        //mediaService.cancelUpload(taskId);
                                }
                        });
                } catch (Exception e) {
                        LOG.e(TAG, "get arg error");
                        callbackContext.error("get arg error");
                        return true;
                }

                return true;
        }

        protected boolean openLoginBox(CallbackContext callbackContext) {
                cordova.getActivity().runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                        OpenAccountUIService openAccountUIService = AlibabaSDK.getService(OpenAccountUIService.class);
                        openAccountUIService.showLogin(cordova.getActivity(), new LoginCallback() {
                                @Override
                                public void onSuccess(OpenAccountSession session) {
                                        //Toast.makeText(cordova.getActivity(), "success: " + session.getUserId(), Toast.LENGTH_SHORT).show();
                                        JSONObject info = new JSONObject();
                                        try {
                                                info.put("token", session.getAuthorizationCode());
                                                info.put("account", session.getUserId());
                                                info.put("data", new JSONObject(session.getOtherInfo()));
                                        } catch (JSONException e) {
                                                LOG.e(TAG, "make json error");
                                        }
                                        currentCallbackContext.success(info);
                                }
                                @Override
                                public void onFailure(int code, String msg) {
                                        //Toast.makeText(cordova.getActivity(), "error: " + msg, Toast.LENGTH_SHORT).show();
                                        currentCallbackContext.error(msg);
                                }
                        });
                        }
                });

                currentCallbackContext = callbackContext;

                return true;
        }

        protected boolean bindAccount(JSONArray args, CallbackContext callbackContext) {
                try {
                        final String account = args.getString(0);
                        cordova.getActivity().runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                        CloudPushService cloudPushService = AlibabaSDK.getService(CloudPushService.class);
                                        cloudPushService.bindAccount(account, new CommonCallback() {
                                                @Override
                                                public void onSuccess() {
                                                        LOG.d(TAG, "bind account success");
                                                        currentCallbackContext.success();
                                                }
                                                @Override
                                                public void onFailed(String errorCode, String errorMessage) {
                                                        LOG.d(TAG, "bind account fail" + "err:" + errorCode + " - message:" + errorMessage);
                                                        currentCallbackContext.error(errorMessage);
                                                }
                                        });
                                }
                        });
                } catch (Exception e) {
                        LOG.e(TAG, "get arg error");
                        callbackContext.error("get arg error");
                        return true;
                }

                currentCallbackContext = callbackContext;
                return true;
        }

        protected boolean unbindAccount(JSONArray args, CallbackContext callbackContext) {
                cordova.getActivity().runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                                CloudPushService cloudPushService = AlibabaSDK.getService(CloudPushService.class);
                                cloudPushService.unbindAccount(new CommonCallback() {
                                        @Override
                                        public void onSuccess() {
                                                LOG.d(TAG, "unbind account success");
                                                currentCallbackContext.success();
                                        }
                                        @Override
                                        public void onFailed(String errorCode, String errorMessage) {
                                                LOG.d(TAG, "unbind account fail" + "err:" + errorCode + " - message:" + errorMessage);
                                                currentCallbackContext.error(errorMessage);
                                        }
                                });
                        }
                });

                currentCallbackContext = callbackContext;
                return true;
        }

        /**
         * 初始化云推送通道
         */
        private void initCloudChannel() {
                CloudPushService cloudPushService = AlibabaSDK.getService(CloudPushService.class);
                if(cloudPushService != null) {
                        cloudPushService.register(cordova.getActivity(),  new CommonCallback() {
                                @Override
                                public void onSuccess() {
                                        LOG.d(TAG, "init cloudchannel success");
                                }
                                @Override
                                public void onFailed(String errorCode, String errorMessage) {
                                        LOG.d(TAG, "init cloudchannel fail" + "err:" + errorCode + " - message:"+ errorMessage);
                                }
                        });
                }else{
                        LOG.i(TAG, "CloudPushService is null");
                }
        }

        public static void onNotification() {
                String js = "Taobao.fireNotificationReceive();";
                try {
                        instance.webView.sendJavascript(js);
                } catch (NullPointerException e) {
                } catch (Exception e) {
                }
        }

        public static void onNotificationOpen() {
                String js = "Taobao.fireNotificationReceive();";
                try {
                        instance.webView.sendJavascript(js);
                } catch (NullPointerException e) {
                } catch (Exception e) {
                }
        }

        public static void onMessage(String msg) {
                String js = "Taobao.fireMsgReceive('" + msg + "');";
                try {
                        instance.webView.sendJavascript(js);
                } catch (NullPointerException e) {
                } catch (Exception e) {
                }
        }

        protected boolean useNativeTaobao(JSONArray args, CallbackContext callbackContext) {
                if (args.length() < 1) {
                        LOG.d(TAG, "arguments length error");
                        callbackContext.error("arguments length error");
                        return true;
                }
                try {
                        if (args.getBoolean(0)) {
                                TradeConfigs.defaultItemDetailWebViewType = TradeConstants.TAOBAO_NATIVE_VIEW;
                        } else {
                                TradeConfigs.defaultItemDetailWebViewType = TradeConstants.BAICHUAN_H5_VIEW;
                        }
                        callbackContext.success();
                } catch (Exception e) {
                        LOG.e(TAG, "get arg error");
                        callbackContext.error("get arg error");
                        return true;
                }

                return true;
        }

        protected boolean showItemDetailByItemId(JSONArray args, CallbackContext callbackContext) {
                int length = args.length();
                if (length < 1) {
                        LOG.d(TAG, "arguments length error");
                        callbackContext.error("arguments length error");
                        return true;
                }
                try {
                        final String itemId = args.getString(0);
                        cordova.getActivity().runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                        ItemDetailPage itemDetailPage = new ItemDetailPage(itemId, null);
                                        TradeService tradeService = AlibabaSDK.getService(TradeService.class);
                                        tradeService.show(itemDetailPage, null, cordova.getActivity(), null, new TradeProcessCallback() {
                                                @Override
                                                public void onFailure(int code, String msg) {
                                                        //Toast.makeText(cordova.getActivity(), "失败 "+code+msg, Toast.LENGTH_SHORT).show();
                                                        currentCallbackContext.error(msg);
                                                }

                                                @Override
                                                public void onPaySuccess(TradeResult tradeResult) {
                                                        //Toast.makeText(cordova.getActivity(), "成功", Toast.LENGTH_SHORT).show();
                                                        currentCallbackContext.success(getTradeResult(tradeResult));
                                                }
                                        });
                                }
                        });
                } catch (Exception e) {
                        LOG.e(TAG, "get itemId error");
                        callbackContext.error("get itemId error");
                        return true;
                }

                currentCallbackContext = callbackContext;

                return true;
        }

        protected boolean showTaoKeItemDetailByItemId(JSONArray args, CallbackContext callbackContext) {
                int length = args.length();
                if (length < 1) {
                        LOG.d(TAG, "arguments length error");
                        callbackContext.error("arguments length error");
                        return true;
                }
                try {
                        final String itemId = args.getString(0);
                        cordova.getActivity().runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                        ItemDetailPage itemDetailPage = new ItemDetailPage(itemId, null);
                                        TaokeParams taokeParams = new TaokeParams();
                                        taokeParams.pid = TAOKEPID;
                                        TradeService tradeService = AlibabaSDK.getService(TradeService.class);
                                        tradeService.show(itemDetailPage, taokeParams, cordova.getActivity(), null, new TradeProcessCallback() {
                                                @Override
                                                public void onFailure(int code, String msg) {
                                                        //Toast.makeText(cordova.getActivity(), "失败 "+code+msg, Toast.LENGTH_SHORT).show();
                                                        currentCallbackContext.error(msg);
                                                }
                                                @Override
                                                public void onPaySuccess(TradeResult tradeResult) {
                                                        //Toast.makeText(cordova.getActivity(), "成功", Toast.LENGTH_SHORT).show();
                                                        currentCallbackContext.success(getTradeResult(tradeResult));
                                                }
                                        });
                                }
                        });
                } catch (Exception e) {
                        LOG.e(TAG, "get itemId error");
                        callbackContext.error("get itemId error");
                        return true;
                }

                currentCallbackContext = callbackContext;

                return true;
        }

        protected boolean showCart(CallbackContext callbackContext) {
                cordova.getActivity().runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                                MyCartsPage myCartsPage = new MyCartsPage();
                                TradeService tradeService = AlibabaSDK.getService(TradeService.class);
                                tradeService.show(myCartsPage, null, cordova.getActivity(), null, new TradeProcessCallback() {
                                        @Override
                                        public void onFailure(int code, String msg) {
                                                //Toast.makeText(cordova.getActivity(), "失败 "+code+msg, Toast.LENGTH_SHORT).show();
                                                currentCallbackContext.error(msg);
                                        }
                                        @Override
                                        public void onPaySuccess(TradeResult tradeResult) {
                                                //Toast.makeText(cordova.getActivity(), "成功", Toast.LENGTH_SHORT).show();
                                                currentCallbackContext.success(getTradeResult(tradeResult));
                                        }
                                });
                        }
                });

                currentCallbackContext = callbackContext;

                return true;
        }

        protected boolean showOrder(CallbackContext callbackContext) {
                cordova.getActivity().runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                                MyOrdersPage myOrdersPage = new MyOrdersPage(0, true);
                                TradeService tradeService = AlibabaSDK.getService(TradeService.class);
                                tradeService.show(myOrdersPage, null, cordova.getActivity(), null, new TradeProcessCallback() {
                                        @Override
                                        public void onFailure(int code, String msg) {
                                                //Toast.makeText(cordova.getActivity(), "失败 "+code+msg, Toast.LENGTH_SHORT).show();
                                                currentCallbackContext.error(msg);
                                        }
                                        @Override
                                        public void onPaySuccess(TradeResult tradeResult) {
                                                //Toast.makeText(cordova.getActivity(), "成功", Toast.LENGTH_SHORT).show();
                                                currentCallbackContext.success(getTradeResult(tradeResult));
                                        }
                                });
                        }
                });

                currentCallbackContext = callbackContext;

                return true;
        }

        protected boolean showPage(JSONArray args, CallbackContext callbackContext) {
                if (args.length() < 1) {
                        LOG.d(TAG, "arguments length error");
                        callbackContext.error("arguments length error");
                        return true;
                }
                try {
                        final String url = args.getString(0);
                        cordova.getActivity().runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                        Page page = new Page(url);
                                        TradeService tradeService = AlibabaSDK.getService(TradeService.class);
                                        tradeService.show(page, null, cordova.getActivity(), null, new TradeProcessCallback() {
                                                @Override
                                                public void onFailure(int code, String msg) {
                                                        //Toast.makeText(cordova.getActivity(), "失败 "+code+msg, Toast.LENGTH_SHORT).show();
                                                        currentCallbackContext.error(msg);
                                                }
                                                @Override
                                                public void onPaySuccess(TradeResult tradeResult) {
                                                        //Toast.makeText(cordova.getActivity(), "成功", Toast.LENGTH_SHORT).show();
                                                        currentCallbackContext.success(getTradeResult(tradeResult));
                                                }
                                        });
                                }
                        });
                } catch (Exception e) {
                        LOG.e(TAG, "get url error");
                        callbackContext.error("get url error");
                        return true;
                }

                currentCallbackContext = callbackContext;

                return true;
        }

        protected JSONObject getTradeResult(TradeResult tradeResult) {
                JSONObject tr = new JSONObject();
                try {
                        JSONArray so = new JSONArray();
                        JSONArray fo = new JSONArray();
                        for (int i = 0; i < tradeResult.paySuccessOrders.size(); ++i) {
                                so.put( Long.toString(tradeResult.paySuccessOrders.get(i)) );
                        }
                        for (int i = 0; i < tradeResult.payFailedOrders.size(); ++i) {
                                fo.put( Long.toString(tradeResult.payFailedOrders.get(i)) );
                        }
                        tr.put("successOrders", so);
                        tr.put("failedOrders", fo);
                } catch (JSONException e) {
                        LOG.e(TAG, "make json error");
                }
                return tr;
        }

        protected JSONObject getUserInfo(User user) {
                JSONObject ui = new JSONObject();
                try {
                        ui.put("id", user.id);
                        ui.put("nick", user.nick);
                        ui.put("avatarUrl", user.avatarUrl);
                } catch (JSONException e) {
                        LOG.e(TAG, "make json error");
                }
                return ui;
        }

        /*
        private void initServices() {
        }

        private void initSSOLoginHandler() {
                LoginService loginService = AlibabaSDK.getService(LoginService.class);
                loginService.setSessionListener(new SessionListener() {
                        @Override
                        public void onStateChanged(Session session) {
                                if (session.isLogin()) {
                                        //Toast.makeText(cordova.getActivity(), "登入", Toast.LENGTH_SHORT).show();
                                } else {
                                        //Toast.makeText(cordova.getActivity(), "登出", Toast.LENGTH_SHORT).show();
                                }
                        }
                });
        }

        protected boolean logout(CallbackContext callbackContext) {
                cordova.getActivity().runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                                LoginService loginService = AlibabaSDK.getService(LoginService.class);
                                loginService.logout(cordova.getActivity(), new LogoutCallback() {
                                        @Override
                                        public void onFailure(int code, String msg) {
                                                //Toast.makeText(cordova.getActivity(), "登出失败", Toast.LENGTH_SHORT).show();
                                                currentCallbackContext.error(msg);
                                        }
                                        @Override
                                        public void onSuccess() {
                                                //Toast.makeText(cordova.getActivity(), "登出成功", Toast.LENGTH_SHORT).show();
                                                currentCallbackContext.success();
                                        }
                                });
                        }
                });

                currentCallbackContext = callbackContext;

                return true;
        }

        protected boolean showLogin(CallbackContext callbackContext) {
                cordova.getActivity().runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                                LoginService loginService = AlibabaSDK.getService(LoginService.class);
                                loginService.showLogin(cordova.getActivity(), new LoginCallback() {
                                        @Override
                                        public void onSuccess(Session session) {
                                            //Toast.makeText(cordova.getActivity(), "欢迎"+session.getUser().nick+session.getUser().avatarUrl, Toast.LENGTH_SHORT).show();
                                            currentCallbackContext.success(getUserInfo(session.getUser()));
                                        }

                                        @Override
                                        public void onFailure(int code, String msg) {
                                            //Toast.makeText(cordova.getActivity(), "失败 "+code+msg, Toast.LENGTH_SHORT).show();
                                            currentCallbackContext.error(msg);
                                        }
                                });
                        }
                });

                currentCallbackContext = callbackContext;

                return true;
        }
        */
}
