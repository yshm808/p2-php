<?php
// p2 -  �g�їp�C���f�b�N�X�v�����g�֐�

require_once './p2util.class.php';	// p2�p�̃��[�e�B���e�B�N���X

function index_print_k()
{
	global $_conf, $login;
	global $_info_msg_ht;
	global $doctype, $accesskey;
	global $k_at_a, $k_at_q;
	
	$newtime = date("gis");
	
	$body = "";
	$autho_user_ht = "";
	$ptitle = "��޷��p2";

	// �F�؃��[�U���
	$autho_user_ht = "";
	if ($login['use']) {
		$autho_user_ht = "<p>۸޲�հ��: {$login['user']} - ".date("Y/m/d (D) G:i:s")."</p>\n";
	}
	
	// �O��̃��O�C�����
	if ($_conf['login_log_rec'] && $_conf['last_login_log_show'] and ($alog = P2Util::getLastAccessLog($_conf['login_log_file']))) {
		$last_login_ht =<<<EOP
�O��̃��O�C����� - {$alog['date']}<br>
���[�U: {$alog['user']}<br>
IP: {$alog['ip']}<br>
HOST: {$alog['host']}<br>
UA: {$alog['ua']}<br>
REFERER: {$alog['referer']}
EOP;
	} else {
		$last_login_ht = "";
	}

	//=========================================================
	// �g�їp HTML �v�����g
	//=========================================================
	header_content_type();
	if($doctype){
		echo $doctype;
	}
	echo <<<EOP
<html>
<head>
	<meta name="ROBOTS" content="NOINDEX, NOFOLLOW">
	<title>{$ptitle}</title>
</head>
<body>
<h1>{$ptitle}</h1>
{$_info_msg_ht}
<ol>
	<li><a {$accesskey}="1" href="subject.php?spmode=fav&amp;sb_view=shinchaku{$k_at_a}">���C�ɽڂ̐V��</a></li>
	<li><a {$accesskey}="2" href="subject.php?spmode=fav{$k_at_a}">���C�ɽڂ̑S��</a></li>
	<li><a {$accesskey}="3" href="menu_k.php?view=favita{$k_at_a}">���C�ɔ�</a></li>
	<li><a {$accesskey}="4" href="menu_k.php?view=cate{$k_at_a}">��ؽ�</a></li>	
	<li><a {$accesskey}="5" href="subject.php?spmode=recent&amp;sb_view=shinchaku{$k_at_a}">�ŋߓǂ񂾽ڂ̐V��</a></li>
	<li><a {$accesskey}="6" href="subject.php?spmode=recent{$k_at_a}">�ŋߓǂ񂾽ڂ̑S��</a></li>
	<li><a {$accesskey}="7" href="subject.php?spmode=res_hist{$k_at_a}">�������ݗ���</a> <a href="read_res_hist.php?nt={$newtime}{$k_at_a}#footer">*</a></li>
	<li><a {$accesskey}="8" href="subject.php?spmode=palace&amp;norefresh=true{$k_at_a}">�ڂ̓a��</a></li>
	<li><a {$accesskey}="9" href="setting.php{$k_at_q}">�ݒ�</a></li>	
</ol>
<br>
{$autho_user_ht}
{$last_login_ht}
</body>
</html>
EOP;

}
?>