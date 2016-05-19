/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
*/

var exec = require('cordova/exec');

var _okCb = function () { console.log('ok'); };
var _errCb = function () { console.log('error'); };
module.exports = {
    useNativeTaobao: function (isNative) {
        exec(null, null, "Taobao", "useNativeTaobao", [!!isNative]);
    },
    logout: function (okCb, errCb) {
        okCb = okCb || _okCb;
        errCb = errCb || _errCb;
        exec(okCb, errCb, "Taobao", "logout", []);
    },
    showLogin: function (okCb, errCb) {
        okCb = okCb || _okCb;
        errCb = errCb || _errCb;
        exec(okCb, errCb, "Taobao", "showLogin", []);
    },
    showItemDetailByItemId: function (itemId, itemType, okCb, errCb) {
        okCb = okCb || _okCb;
        errCb = errCb || _errCb;
        exec(okCb, errCb, "Taobao", "showItemDetailByItemId", [itemId, itemType]);
    },
    showTaoKeItemDetailByItemId: function (itemId, itemType, okCb, errCb) {
        okCb = okCb || _okCb;
        errCb = errCb || _errCb;
        exec(okCb, errCb, "Taobao", "showTaoKeItemDetailByItemId", [itemId, itemType]);
    },
    showCart: function () {
        exec(null, null, "Taobao", "showCart", []);
    },
    showOrder: function () {
        exec(null, null, "Taobao", "showOrder", []);
    },
    showPage: function (url, okCb, errCb) {
        okCb = okCb || _okCb;
        errCb = errCb || _errCb;
        exec(okCb, errCb, "Taobao", "showPage", [url]);
    },
    openLoginBox: function (okCb, errCb) {
        okCb = okCb || _okCb;
        errCb = errCb || _errCb;
        exec(okCb, errCb, "Taobao", "openLoginBox", []);
    },
    uploadImage: function (imageId, dir, fileName, imgType, okCb, errCb) {
        okCb = okCb || _okCb;
        errCb = errCb || _errCb;
        exec(okCb, errCb, "Taobao", "uploadImage", [imageId, dir, fileName, imgType]);
    },
    shareTo: function (platform, text, imgUrl, title, url, mode, okCb, errCb) {
        okCb = okCb || _okCb;
        errCb = errCb || _errCb;
        exec(okCb, errCb, "Taobao", "shareTo", [platform, text, imgUrl, title, url, mode]);
    },
    fireNotificationReceive: function () {
        cordova.fireWindowEvent('Taobao.notificationReceive');
    }
};

