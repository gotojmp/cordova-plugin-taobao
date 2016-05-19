package com.gotojmp.cordova.taobao;

import android.widget.Toast;

import com.alibaba.sdk.android.AlibabaSDK;
import com.alibaba.sdk.android.callback.InitResultCallback;
import com.alibaba.sdk.android.push.CloudPushService;
import com.alibaba.sdk.android.push.CommonCallback;
import com.alibaba.sdk.android.login.LoginService;
import com.alibaba.sdk.android.login.callback.LogoutCallback;
import com.alibaba.sdk.android.login.callback.LoginCallback;

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

        private static Taobao instance;

        private static CallbackContext currentCallbackContext;

        private String TAOKEPID;

        public Taobao() {
                instance = this;
        }

        @Override
        protected void pluginInitialize() {
                super.pluginInitialize();

                TradeConfigs.defaultItemDetailWebViewType = TradeConstants.BAICHUAN_H5_VIEW;
                TradeConfigs.defaultISVCode = "99jun-app";
                TAOKEPID = preferences.getString(PIDKEY, "");

                AlibabaSDK.asyncInit(cordova.getActivity(), new InitResultCallback() {
                        @Override
                        public void onSuccess() {
                                //Toast.makeText(cordova.getActivity(), "初始化成功", Toast.LENGTH_SHORT).show();
                                initCloudChannel();
                        }
                        @Override
                        public void onFailure(int code, String msg) {
                                //Toast.makeText(cordova.getActivity(), "初始化异常"+code+msg, Toast.LENGTH_SHORT).show();
                        }
                });
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

        @Override
        public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {
                LOG.d(TAG, action + " is called.");

                if (action.equals("useNativeTaobao")) {
                        return useNativeTaobao(args, callbackContext);
                } else if (action.equals("logout")) {
                        return logout(callbackContext);
                } else if (action.equals("showLogin")) {
                        return showLogin(callbackContext);
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
                }
                return super.execute(action, args, callbackContext);
        }

        /**
         * 初始化云推送通道
         * @param applicationContext
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
}
