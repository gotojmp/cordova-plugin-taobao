package com.gotojmp.cordova.taobao;

import android.widget.Toast;

import com.alibaba.sdk.android.AlibabaSDK;
import com.alibaba.sdk.android.callback.InitResultCallback;
import com.alibaba.sdk.android.push.CloudPushService;
import com.alibaba.sdk.android.push.MessageReceiver;
import com.alibaba.sdk.android.push.CommonCallback;

import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.CallbackContext;
import org.apache.cordova.LOG;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.content.Context;
import java.util.Map;

public class TaobaoMessageReceiver extends MessageReceiver {

        @Override
        protected void onNotification(Context context, String title, String body, Map<String, String> extraMap) {
                super.onNotification(context, title, body, extraMap);
                Taobao.onNotification();
        }

        @Override
        protected void onNotificationOpened(Context context, String title, String body, String extraMap) {
                super.onNotificationOpened(context, title, body, extraMap);
                Taobao.onNotificationOpen();
        }
}
