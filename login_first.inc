<?php
// p2 ���O�C��

require_once("./filectl_class.inc");
require_once("./datactl.inc");
require_once("./login.inc");

$_info_msg_ht="";

if( isset($_GET['regist_cookie']) ){ $regist_cookie = $_GET['regist_cookie']; }
if( isset($_POST['regist_cookie']) ){ $regist_cookie = $_POST['regist_cookie']; }

//�p�[�~�b�V�������ӊ��N================
if($prefdir==$datdir){
	datadir_writable_check($prefdir);
}else{
	datadir_writable_check($prefdir);
	datadir_writable_check($datdir);
}

//=========================================================
// �����o���p�ϐ�
//=========================================================
$ptitle="p2";

$auth_sub_input_ht="";
$body_ht="";

if($ktai){
	$user_st="հ��";
	$password_st="�߽ܰ��";
	
}else{
	$user_st="���[�U";
	$password_st="�p�X���[�h";
}


//�⏕�F��================================
//EZ�F��===============
if($_SERVER['HTTP_X_UP_SUBNO']){
	if( file_exists($auth_ez_file) ){
	}else{
		$auth_sub_input_ht=<<<EOP
	<input type="checkbox" name="regist_ez" value="in" checked>EZ�[��ID�ŔF�؂�o�^<br>
EOP;
	}

//J�F��================
}elseif( preg_match("/J-PHONE\/[^\/]+\/[^\/]+\/SN(.+?) /", $_SERVER['HTTP_USER_AGENT'], $matches) ){
	if( file_exists($auth_jp_file) ){
	}else{
		$auth_sub_input_ht=<<<EOP
	<input type="checkbox" name="regist_jp" value="in" checked>J�[��ID�ŔF�؂�o�^<br>
EOP;
	}
	
//Cookie�F��================
}else{
	$auth_sub_input_ht = <<<EOP
	<input type="checkbox" name="regist_cookie" value="in" checked>cookie�ɕۑ�����<br>
EOP;
}

//���O�C���p�t�H�[��======
$login_form_ht =<<<EOP
<p>�F��{$user_st}��{$password_st}��V�K�o�^���܂�</p>
<form id="login" method="POST" action="{$_SERVER['REQUEST_URI']}" target="_self">
	{$k_input_ht}
	{$user_st}: <input type="text" name="login_user" value="{$_POST['login_user']}"><br>
	{$password_st}: <input type="password" name="login_pass" value="{$_POST['login_pass']}"><br>
	{$auth_sub_input_ht}
	<br>
	<input type="submit" name="submit" value="�V�K�o�^">
</form>\n
EOP;




//���[�U�o�^����=================================
if($_POST['login_user'] && $_POST['login_pass']){

	if( isStrInvalid($_POST['login_user']) || isStrInvalid($_POST['login_pass']) ){
		$_info_msg_ht.="<p class=\"infomsg\">p2 error: {$user_st}����{$password_st}�͔��p�p�����œ��͂��ĉ������B</p>";
		$show_login_form = true;
	
	}else{
		$crypted_login_pass = crypt($_POST['login_pass']);
		$auth_user_cont =<<<EOP
<?php
\$login['user']='{$_POST["login_user"]}';
\$login['pass']='{$crypted_login_pass}';
?>
EOP;
		FileCtl::make_datafile($auth_user_file, $pass_perm); //$auth_user_file ���Ȃ���ΐ���
		$fp = @fopen($auth_user_file,"w") or die("p2 error: $auth_user_file ��ۑ��ł��܂���ł����B�F��{$user_st}�o�^���s�B");
		fputs($fp, $auth_user_cont);
		fclose($fp);
	
		//�o�^����======================================================
		
		//�⏕�F��==================================================
		regist_set_ktai($auth_ez_file, $auth_jp_file);
		regist_set_cookie();

		$body_ht .= "<p class=\"infomsg\">�� �F��{$user_st}�u{$_POST['login_user']}�v��o�^���܂���</p>";
		$body_ht .= "<p><a href=\"{$_SERVER['REQUEST_URI']}{$k_at_q}\">p2 start</a></p>";
		
	}
	
}else{
	
	if($_POST['login_user'] || $_POST['login_pass']){
		if(!$_POST['login_user']){
			$_info_msg_ht.="<p class=\"infomsg\">p2 error: {$user_st}�������͂���Ă��܂���B</p>";
		}elseif(!$_POST['login_pass']){
			$_info_msg_ht.="<p class=\"infomsg\">p2 error: {$password_st}�����͂���Ă��܂���B</p>";
		}
	}
	$show_login_form = true;

}


//=========================================================
// HTML�v�����g
//=========================================================
header_nocache();
header_content_type();
if($doctype){ echo $doctype;}
echo <<<EOP
<html>
<head>
	<meta name="ROBOTS" content="NOINDEX, NOFOLLOW">
	<meta http-equiv="Content-Style-Type" content="text/css">
	<meta http-equiv="Content-Script-Type" content="text/javascript">
	<title>{$ptitle}</title>
EOP;
if(!$ktai){
	@include("./style/style_css.inc");
	@include("./style/login_first_css.inc");
}
echo <<<EOP
</head>
<body>
EOP;
echo <<<EOP
<h2>{$ptitle}</h2>
EOP;

echo $_info_msg_ht;
echo $body_ht;

if($show_login_form){
	echo $login_form_ht;
}

echo <<<EOP
</body>
</html>
EOP;

?>