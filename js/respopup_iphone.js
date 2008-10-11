/*
	rep2expack - iPhone用レスポップアップ
*/

// {{{ globals

var _RESPOPUP_IPHONE_JS_HASH = new Object();
var _RESPOPUP_IPHONE_JS_INDEX = 0;
var _RESPOPUP_IPHONE_JS_XPATH = './/div[@class="res"]' +
	'//a[starts-with(@href, "read.php?") or starts-with(@href, "subject.php?")]';

// }}}
// {{{ _irespopup_get_z_index()

/*
 * z-indexに設定する値を返す
 *
 * css/ic2_iphone.css で div#ic2-info の z-index が 999 で
 * 固定されているのでポップアップを繰り返すと不具合がある。
 * ポップアップオブジェクトの z-index を集中管理する必要あり。
 *
 * @param Element obj
 * @return String
 */
function _irespopup_get_z_index(obj)
{
	return (10 + _RESPOPUP_IPHONE_JS_INDEX).toString();
}

// }}}
// {{{ _irespopup_make_activate()

/*
 * オブジェクトを最前面に移動する関数を返す
 *
 * @param Element obj
 * @return void
 */
function _irespopup_make_activate(obj)
{
	return (function(){
		_RESPOPUP_IPHONE_JS_INDEX++;
		obj.style.zIndex = _irespopup_get_z_index();
	});
}

// }}}
// {{{ _irespopup_make_deactivate()

/*
 * DOMツリーからオブジェクトを取り除く関数を返す
 *
 * @param Element obj
 * @param Strin key 
 * @return void
 */
function _irespopup_make_deactivate(obj, key)
{
	return (function(){
		delete _RESPOPUP_IPHONE_JS_HASH[key];
		obj.parentNode.removeChild(obj);
		delete obj;
	});
}

// }}}
// {{{ iResPopUp()

/*
 * iPhone用レスポップアップ
 *
 * @param String url
 * @param Event evt
 * @return Boolean
 * @todo use asynchronous request
 */
function iResPopUp(url, evt)
{
	var yOffset = Math.max(10, evt.getOffsetY() - 20);
	var cbox = document.getElementById('open-in-tab-cbox');

	if (_RESPOPUP_IPHONE_JS_HASH[url]) {
		_RESPOPUP_IPHONE_JS_INDEX++;
		_RESPOPUP_IPHONE_JS_HASH[url].style.top = yOffset.toString() + 'px';
		_RESPOPUP_IPHONE_JS_HASH[url].style.zIndex = _irespopup_get_z_index();
		if (cbox && cbox.checked) {
			change_link_target(_RESPOPUP_IPHONE_JS_XPATH, true, _RESPOPUP_IPHONE_JS_HASH[url]);
		}
		return false;
	}

	var req = new XMLHttpRequest();
	req.open('GET', url + '&ajax=true', false);
	req.send(null);

	if (req.readyState == 4) {
		if (req.status == 200) {
			_RESPOPUP_IPHONE_JS_INDEX++;

			var container = document.createElement('div');
			var closer = document.createElement('img');
			var popid = '_respop' + _RESPOPUP_IPHONE_JS_INDEX.toString();

			container.id = popid;
			container.className = 'respop';
			container.innerHTML = req.responseText
				.replace(/^<div class="thread">/, '')
				.replace(/<\/div>\s*$/, '')
				.replace(/<[^<>]+? id="/, '$0' + popid + '_'); //"
			container.style.top = yOffset.toString() + 'px';
			container.style.zIndex = _irespopup_get_z_index();
			//container.onclick = _irespopup_make_activate(container);

			closer.className = 'close-button';
			closer.setAttribute('src', 'img/iphone/close.png');
			closer.onclick = _irespopup_make_deactivate(container, url);

			container.appendChild(closer);
			document.body.appendChild(container);

			rewrite_external_link(container);

			_RESPOPUP_IPHONE_JS_HASH[url] = container;

			if (cbox && cbox.checked) {
				change_link_target(_RESPOPUP_IPHONE_JS_XPATH, true, container);
			}

			var lastres = document.evaluate('./div[@class="res" and position() = last()]',
			                                container,
			                                null,
			                                XPathResult.ANY_UNORDERED_NODE_TYPE,
			                                null
			                                ).singleNodeValue;

			if (lastres) {
				var back = document.createElement('div');
				back.className = 'respop-back';
				var anchor = document.createElement('a');
				anchor.setAttribute('href', '#' + popid);
				anchor.onclick = (function(){
					scrollTo(0, yOffset - 10);
					return false;
				});
				anchor.innerText = '▲';
				back.appendChild(anchor);
				lastres.appendChild(back);
			}

			return false;
		}
	}

	return true;
}

// }}}

/*
 * Local Variables:
 * mode: javascript
 * coding: cp932
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
/* vim: set syn=javascript fenc=cp932 ai noet ts=4 sw=4 sts=4 fdm=marker: */
