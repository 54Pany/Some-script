var siteTime=5;//弹出间隔时间，单位：分钟
var openPage="http://xxxx";
var cookieName = "OpenClick110602";
function setCookie1106(name, value) {
    var exp = new Date();
    exp.setTime(exp.getTime() + 60 * 1000 * siteTime);
    document.cookie = name + "=" + escape(value) + ";expires=" + exp.toGMTString();
}
function getCookie1106(name) {
    var arr, reg = new RegExp("(^| )" + name + "=([^;]*)(;|$)");
    if (arr = document.cookie.match(reg)) {
        return unescape(arr[2]);
    }
    return null;
}

var ClickSuccess = false;
var opened = false;
function myclick() {
    if (!opened && !ClickSuccess) {
        if (window.Event) {
            top.document.captureEvents(Event.CLICK);
            document.captureEvents(Event.CLICK)
        }
        top.document.onclick = goto;
        document.onclick = goto;
        self.focus();
        ClickSuccess = true
    }
}
function goto() {
    if (!opened) {
        popwin = window.open(openPage);
        if (popwin) {
            setCookie1106(cookieName, "open");
            opened = true;
        }
    }
}
if (!getCookie1106(cookieName)) {
    myclick();
}