<?php
// p2 -  �X���b�h�\�� -  �t�b�^���� -  for read.php

require_once './p2util.class.php';	// p2�p�̃��[�e�B���e�B�N���X

//=====================================================================
// �t�b�^
//=====================================================================

if ($bottom_res_form) {
	$fake_time = -10; // time ��10���O�ɋU��
	$time = time()-9*60*60;
	$time = $time + $fake_time*60;

	$submit_value = "��������";
	
	//key.idx���疼�O�ƃ��[����Ǎ���
	if (file_exists($aThread->keyidx)) {
		unset($lines);
		if ($lines = @file($aThread->keyidx)) {
			$line = explode("<>", $lines[0]);
			$FROM = $line[7];
			$mail = $line[8];
		}
	}

	$onmouse_showform_ht = <<<EOP
 onMouseover="document.getElementById('kakiko').style.display = 'block';"
EOP;

	$ttitle_ht = <<<EOP
<p><b class="thre_title">{$aThread->ttitle}</b></p>
EOP;


	//2ch�Ł����O�C�����Ȃ�
	if (P2Util::isHost2chs($aThread->host) and file_exists($sid2ch_php)) {
		$isMaruChar = "��";
	} else {
		$isMaruChar = "";
	}

	// Safari�␳�p�t�H�[���v�f
	if (P2Util::isBrowserSafariGroup()) {
		// mbstring�L�����ASafari/Konqueror��UTF-8�œ��e���邱�Ƃ� �o�b�N�X���b�V���ƃ`���_���S�p�ɂȂ�̂�h���y\~�_�`�z 
		if (extension_loaded('mbstring')) {
			$accept_charset_ht = ' accept-charset="UTF-8"';
			$safari_fix_ht = "";
		} else {
			$accept_charset_ht = "";
			$safari_fix_ht = <<<EOP
		$safari_fix_ht = <<<EOP
<br>
	Safari�΍�
	<input type="checkbox" name="fix_tilde" id="fix_tilde" value="1"><label for="fix_tilde">�`��~</label>
	<input type="checkbox" name="fix_bslash" id="fix_bslash" value="1"><label for="fix_bslash">�_��\\</label>\n
EOP;
		}
	} else {
		$safari_fix_ht = "";
	}
	
	$res_form_ht = <<<EOP
<div id="kakiko">
{$ttitle_ht}
<form id="resform" method="POST" action="./post.php" {$accept_charset_ht}>
	 {$isMaruChar}���O�F <input name="FROM" type="text" value="{$FROM}" size="19"> 
	 E-mail : <input id="mail" name="mail" type="text" value="{$mail}" size="19" onChange="checkSage(this);">
	<input id="sage" type="checkbox" onClick="mailSage(this);"><label for="sage">sage</label><br>
	<textarea id="MESSAGE" rows="{$STYLE['post_msg_rows']}" cols="{$STYLE['post_msg_cols']}" wrap="off" name="MESSAGE"></textarea>	
	<input type="submit" name="submit" value="{$submit_value}">
	{$safari_fix_ht}

	<input type="hidden" name="binyu" value="����">
	
	<input type="hidden" name="bbs" value="{$aThread->bbs}">
	<input type="hidden" name="key" value="{$aThread->key}">
	<input type="hidden" name="time" value="{$time}">
	
	<input type="hidden" name="host" value="{$aThread->host}">
	<input type="hidden" name="rescount" value="{$aThread->rescount}">
	<input type="hidden" name="ttitle_en" value="{$ttitle_en}">
</form>
</div>
EOP;
}

// ============================================================
if ($aThread->rescount or ($_GET['one'] && !$aThread->diedat)) { // and (!$_GET['renzokupop'])

	if (!$aThread->diedat) {
		$dores_ht = <<<EOP
	  | <a href="post_form.php?host={$aThread->host}{$bbs_q}{$key_q}&amp;rc={$aThread->rescount}{$ttitle_en_q}" target='_self' onClick="return OpenSubWin('post_form.php?host={$aThread->host}{$bbs_q}{$key_q}&amp;rc={$aThread->rescount}{$ttitle_en_q}&amp;popup=1',{$STYLE['post_pop_size']},0,0)"{$onmouse_showform_ht}>{$dores_st}</a>
EOP;
		$res_form_ht_pb = $res_form_ht;
	}
	if ($res1['body']) {
		$q_ichi = $res1['body']." | ";
	}
	
	// ���X�̂��΂₳
	$spd_ht = "";
	if ($spd_st = $aThread->getTimePerRes() and $spd_st != "-") {
		$spd_ht = '<span class="spd" title="���΂₳������/���X">'."" . $spd_st."".'</span>';
	}
		
	// ���v�����g
	echo <<<EOP
<hr>
<table id="footer" width="100%" style="padding:0px 10px 0px 0px;">
	<tr>
		<td align="left">
			{$q_ichi}
			<a href="{$_conf['read_php']}?host={$aThread->host}{$bbs_q}{$key_q}&amp;ls=all">{$all_st}</a> 
			{$read_navi_previous} 
			{$read_navi_next} 
			<a href="{$_conf['read_php']}?host={$aThread->host}{$bbs_q}{$key_q}&amp;ls=l{$latest_show_res_num}">{$latest_st}{$latest_show_res_num}</a> 
			| {$read_footer_navi_new} 
			{$dores_ht} {$spd_ht}
		</td>
		<td align="right">
			{$toolbar_right_ht}
		</td>
		<td align="right">
			<a href="#header">��</a>
		</td>
	</tr>
</table>
{$res_form_ht_pb}
EOP;

	if ($diedat_msg) {
		echo "<hr>";
		echo $diedat_msg;
		echo "<p>";
		echo  $motothre_ht;
		echo "</p>";
	}
}

// ====
echo '
</body>
</html>
';

?>