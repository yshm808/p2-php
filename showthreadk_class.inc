<?php
// p2 - �X���b�h��\������ �N���X �g�їp

class ShowThreadK extends ShowThread{
	
	function ShowThreadK($aThread){
		$this->thread = $aThread;
	}
	
	//============================================================================
	// datToHtml -- Dat��HTML�ɕϊ��\�����郁�\�b�h
	//============================================================================	
	function datToHtml(){
	
		if(!$this->thread->resrange){echo '<p><b>p2 error: {$this->resrange} is false at datToHtml() in threadread_class.inc</b></p>';}

		$start=$this->thread->resrange['start'];
		$to=$this->thread->resrange['to'];
		$nofirst=$this->thread->resrange['nofirst'];

		$status_title = $this->thread->itaj." / ".$this->thread->ttitle;
		$status_title = str_replace("'", "\'", $status_title);
		$status_title = str_replace('"', "\'\'", $status_title);
		
		if(!$nofirst){ //1��\��
			$cont_ht .= $this->transRes($this->thread->datlines[0], 1);
		}

		for($i=$start; $i<=$to; $i++){
			if(!$nofirst and $i==1){continue;}
			if(!$this->thread->datlines[$i-1]){
				$this->thread->newline=$i; 
				break;
			}
			$cont_ht .= $this->transRes($this->thread->datlines[$i-1], $i);
		}
		
		//$s2e=array($start, $i-1);
		//return $s2e;
		return $cont_ht;
	}


	//============================================================================
	// transRes -- Dat���X��HTML���X�ɕϊ����郁�\�b�h
	// ���� - dat��1���C��, ���X�ԍ�
	//============================================================================
	function transRes($ares,$i)
	{
		global $STYLE, $mae_msg, $field, $res_filter, $word_fm;
		global $aborn_words, $ng_words, $ng_num;
		global $pointer_at, $_conf, $newres_to_show;
		global $k_at_a, $k_at_q, $k_input_ht;
		
		$tores="";
		$rpop="";
		$isNgName=false;
		$isNgMsg=false;
		
		$resar = $this->thread->explodeDatLine($ares);
		$name=$resar[0];
		$mail = $resar[1];
		$date_id = $resar[2];
		$msg=$resar[3];

		if($field){
			if(! $word_fm){return;}

			if($res_filter['field']=="name"){$target = StrCtl::p2SJIStoEUC($name);}
			elseif($res_filter['field']=="mail"){$target = StrCtl::p2SJIStoEUC($mail);}
			elseif($res_filter['field']=="id"){$target = StrCtl::p2SJIStoEUC($date_id);} //���ۂ͓��t���܂܂��
			elseif($res_filter['field']=="msg"){$target= StrCtl::p2SJIStoEUC($msg);}
			if($res_filter['match']=="on"){
				if(! @preg_match("/{$word_fm}/i", $target)){return;}
			}else{
				if(@preg_match("/{$word_fm}/i", $target)){return;}
			}
		}
		
		//���ځ[��`�F�b�N====================================
		$aborned_res .= "<div {$pointer_at}=\"r{$i}\">&nbsp;</div>\n"; //���O
		$aborned_res .= ""; //���e

		//���ځ[��l�[��=================================
		if( $this->aborn($aborn_words['name'], $name, $i) ){
			return $aborned_res;
		}

		//���ځ[�񃁁[��=================================
		if( $this->aborn($aborn_words['mail'], $mail, $i) ){
			return $aborned_res;
		}
		
		//���ځ[��ID===================================
		if( $this->aborn($aborn_words['id'], $date_id, $i) ){
			return $aborned_res;
		}
		
		//���ځ[�񃁃b�Z�[�W==============================
		if( $this->aborn($aborn_words['msg'], $msg, $i) ){
			return $aborned_res;
		}

		if(!$_GET['nong']){
			//NG�l�[���`�F�b�N======================================
			if( $this->ngCheck($ng_words['name'], $name) ){
				$ng_num['name']++;
				$isNgName=true;
			}

			//NG���[���`�F�b�N===============================
			if( $this->ngCheck($ng_words['mail'], $mail) ){
				$ng_num['mail']++;
				$isNgMail=true;
			}
		
			//NGID�`�F�b�N======================================
			if( $this->ngCheck($ng_words['id'], $date_id) ){
				$ng_num['id']++;
				$isNgId=true;
			}
	
			//NG���b�Z�[�W�`�F�b�N======================================
			if( $a_ng_msg = $this->ngCheck($ng_words['msg'], $msg) ){
				$ng_num['msg']++;
				$isNgMsg=true;
			}
		}
		
		//=============================================================
		// �܂Ƃ߂ďo��
		//=============================================================
		
		$name = $this->transName($name); // ���OHTML�ϊ�
		$msg = $this->transMsg($msg, $i); //���b�Z�[�WHTML�ϊ�

		
		//NG���b�Z�[�W�ϊ�======================================
		if($isNgMsg){
			$msg=<<<EOMSG
<s><font color="{$STYLE['read_ngword']}">NGܰ��:{$a_ng_msg}</font></s> <a href="{$_conf['read_php']}?host={$this->thread->host}&amp;bbs={$this->thread->bbs}&amp;key={$this->thread->key}&amp;ls={$i}&amp;k_continue=1&amp;nong=1{$k_at_a}">�m</a>
EOMSG;
		}
		
		//NG�l�[���ϊ�======================================
		if($isNgName){
			$name=<<<EONAME
<s><font color="{$STYLE['read_ngword']}">$name</font></s>
EONAME;
			$msg=<<<EOMSG
<a href="{$_conf['read_php']}?host={$this->thread->host}&amp;bbs={$this->thread->bbs}&amp;key={$this->thread->key}&amp;ls={$i}&amp;k_continue=1&amp;nong=1{$k_at_a}">�m</a>
EOMSG;
		
		//NG���[���ϊ�======================================
		}elseif($isNgMail){
			$mail=<<<EOMAIL
<s class="ngword" onMouseover="document.getElementById('ngn{$ng_num['mail']}').style.display = 'block';">$mail</s>
EOMAIL;
			$msg=<<<EOMSG
<div id="ngn{$ng_num['mail']}" style="display:none;">$msg</div>
EOMSG;

		//NGID�ϊ�======================================
		}elseif($isNgId){
			$date_id=<<<EOID
<s><font color="{$STYLE['read_ngword']}">$date_id</font></s>
EOID;
			$msg=<<<EOMSG
<a href="{$_conf['read_php']}?host={$this->thread->host}&amp;bbs={$this->thread->bbs}&amp;key={$this->thread->key}&amp;ls={$i}&amp;k_continue=1&amp;nong=1{$k_at_a}">�m</a>
EOMSG;
		}
	
		/*
		//�u��������V���v�摜��}��========================
		if($i == $this->thread->newline){
			$tores .=<<<EOP
				<div><img src="img/image.png" alt="�V�����X" border="0" vspace="4"></div>
EOP;
		}
		*/

		if($this->thread->onthefly){ //ontheflyresorder
			$newres_to_show=true;
			$tores .= "<div {$pointer_at}=\"r{$i}\">[<font color=\"#00aa00'\">{$i}</font>]"; //�ԍ��i�I���U�t���C���j
		}elseif($i>=$this->thread->newline){
			$newres_to_show=true;
			$tores .= "<div {$pointer_at}=\"r{$i}\">[<font color=\"{$STYLE['read_newres_color']}\">{$i}</font>]"; //�ԍ��i�V�����X���j
		}else{
			$tores .= "<div {$pointer_at}=\"r{$i}\">[{$i}]"; //�ԍ�			
		}
		$tores .= $name.":"; //���O
		if($mail){$tores .= $mail.": ";} //���[��
		$tores .= $date_id."<br>\n"; //���t��ID
		$tores .= $rpop; //���X�|�b�v�A�b�v�p���p
		$tores .= "{$msg}</div><hr>\n"; //���e
		
		return $tores;
	}
	
	//============================================================================
	// transName -- ���O��HTML�p�ɕϊ����郁�\�b�h
	//============================================================================
	function transName($name)
	{
		global $_conf;
		global $k_at_a, $k_at_q, $k_input_ht;
		$nameID="";

		$name = str_replace("</b>", "", $name);
		$name = str_replace("<b>", "", $name);

		if( preg_match("/(.*)(��.*)/", $name, $matches) ){ //���O
			$name=$matches[1];
			$nameID=$matches[2];
		}

		// 1�������N��
		$name && $name=preg_replace("/([0-9]+)/","<a href=\"{$_conf['read_php']}?host={$this->thread->host}&amp;bbs={$this->thread->bbs}&amp;key={$this->thread->key}&amp;ls=\\1{$k_at_a}\">\\1</a>",$name, 1);
		
		if($nameID){$name = $name . $nameID;}
		
		$name=$name." "; //�����������
	
		return $name;
	}

	
	//============================================================================
	// transMsg --  dat�̃��X���b�Z�[�W��HTML�\���p���b�Z�[�W�ɕϊ����郁�\�b�h
	// string transMsg(string str)
	//============================================================================	
	function transMsg($msg, $mynum)
	{
		global $_conf, $subject_php, $preview_thumbnail, $brocra_checker, $ext_win_target;
		global $field, $res_filter, $word_fm, $p2ime_url;
		global $ktai_res_size, $ktai_ryaku_size;
		global $k_at_a, $k_at_q, $k_input_ht;
		
		$ryaku = false;
		
		//2ch���`����dat
		if($this->thread->dat_type == "2ch_old"){
			$msg = str_replace("���M", ",", $msg);
			$msg = preg_replace("/&amp([^;])/", "&\\1", $msg);
		}

		// >>1�̃����N����������O��
		// <a href="../test/read.cgi/accuse/1001506967/1" target="_blank">&gt;&gt;1</a>
		$msg = preg_replace("/<a href=\"\.\.\/test\/read\.cgi\/{$this->thread->bbs}\/{$this->thread->key}\/([-0-9]+)\" target=\"_blank\">&gt;&gt;([-0-9]+)<\/a>/","&gt;&gt;\\1", $msg);
	
		//�傫������
		if(! $_GET['k_continue']){
			if( strlen($msg) > $ktai_res_size){
				$msg = substr($msg, 0, $ktai_ryaku_size);
				
				//������<br>������Ύ�菜��
				if( substr($msg, -1)==">" ){
					$msg = substr($msg, 0, strlen($msg)-1);
				}
				if( substr($msg, -1)=="r" ){
					$msg = substr($msg, 0, strlen($msg)-1);
				}
				if( substr($msg, -1)=="b" ){
					$msg = substr($msg, 0, strlen($msg)-1);
				}
				if( substr($msg, -1)=="<" ){
					$msg = substr($msg, 0, strlen($msg)-1);
				}
				
				$msg = $msg." ";
				$msg .= "<a href=\"{$_conf['read_php']}?host={$this->thread->host}&amp;bbs={$this->thread->bbs}&amp;key={$this->thread->key}&amp;ls={$mynum}&amp;k_continue=1{$k_at_a}\">��</a>";
				$ryaku=true;
			}
		}
		
		/*
		//�t�B���^�F����
		if($field and $res_filter['field']=="msg" && $word_fm && $res_filter['match']=="on"){
			$msg_euc = StrCtl::p2SJIStoEUC($msg);
			$msg_euc = @preg_replace("/{$word_fm}/i", "<b class='filtering'>\\0</b>", $msg_euc);
			$msg = StrCtl::p2EUCtoSJIS($msg_euc);
		}
		*/
		
		// >>1, >1, ��1, ����1�����p���X�|�b�v�A�b�v�����N��
		$msg = preg_replace_callback("/(&gt;|��)?(&gt;|��)([0-9- ,=.]|�A)+/", array($this, 'quote_res_callback'), $msg);
	
		if ($ryaku) {
			return $msg;
		}
	
		// FTP�����N�̗L����
		$msg = preg_replace("/ftp:\/\/[-_.!~*'a-zA-Z0-9;\/?:@&=+\$,%#]+/","<a href=\"\\0\"{$ext_win_target}>\\0</a>", $msg);
		
		// �ih�������܂߂��jURL�����N�̗L����
		$msg = preg_replace("/([^f])(h?t?)(tps?:\/\/[-_.!~*'a-zA-Z0-9;\/?:@&=+\$,%#]+)/","\\1<a href=\"ht\\3\"{$ext_win_target}>\\2\\3</a>", $msg);
		$msg = preg_replace("/&gt;\"{$ext_win_target}>(.+)&gt;<\/a>/","\"{$ext_win_target}>\\1</a>&gt;", $msg); //������&gt;�i>�j�����O���Ă���
		
		// �T�[�o�������N��p2�\����
		// 2ch bbspink
		// http://choco.2ch.net/test/read.cgi/event/1027770702/
		$msg = preg_replace_callback("/<a href=\"http:\/\/([^\/]+\.(2ch\.net|bbspink\.com))\/test\/read\.cgi\/([^\/]+)\/([0-9]+)(\/)?([^\/]+)?\"{$ext_win_target}>(h?t?tp:\/\/([^\/]+(\.2ch\.net|\.bbspink\.com))\/test\/read\.cgi\/([^\/]+)\/([0-9]+)(\/)?([^\/]+)?)<\/a>/", array($this, 'link2ch_callback'), $msg);
			
		// �܂�BBS / JBBS��������� 
		// http://kanto.machibbs.com/bbs/read.pl?BBS=kana&KEY=1034515019
		// http://jbbs.shitaraba.com/study/bbs/read.cgi?BBS=389&KEY=1036227774&LAST=100
		$ande = "(&|&amp;)";
		$msg = preg_replace_callback("{<a href=\"http://(([^/]+\.machibbs\.com|[^/]+\.machi\.to|jbbs\.livedoor\.jp|jbbs\.livedoor\.com|jbbs\.shitaraba\.com)(/[^/]+)?)/bbs/read\.(pl|cgi)\?BBS=([^&]+)(&|&amp;)KEY=([0-9]+)((&|&amp;)START=([0-9]+))?((&|&amp;)END=([0-9]+))?[^\"]*\"{$_conf['ext_win_target_at']}>(h?t?tp://[^<>]+)</a>}", array($this, 'linkMachi_callback'), $msg);
		$msg = preg_replace_callback("{<a href=\"http://(jbbs\.livedoor\.jp|jbbs\.livedoor\.com|jbbs\.shitaraba\.com)/bbs/read\.cgi/(\w+)/(\d+)/(\d+)/((\d+)?-(\d+)?)?[^\"]*?\"{$_conf['ext_win_target_at']}>(h?t?tp://[^<>]+)</a>}", array($this, 'linkJBBS_callback'), $msg);
		//$msg=preg_replace("/&(amp;)?ls=-/", "", $msg);// ��͈͎̔w��͏���
		
		// 2ch��bbspink�̔�
		$msg = preg_replace("/<a href=\"http:\/\/([^\/]+\.(2ch\.net|bbspink\.com))\/([^\/]+)\/\"{$ext_win_target}>h?t?tp:\/\/([^\/]+(\.2ch\.net|\.bbspink\.com))\/([^\/]+)\/<\/a>/", "\\0 [<a href=\"{$subject_php}?host=\\1&amp;bbs=\\3{$k_at_a}\">��p2�ŊJ��</a>]", $msg);
		
		//2ch��bbspink�̉ߋ����O
		$msg = preg_replace_callback("/<a href=\"(http:\/\/([^\/]+\.(2ch\.net|bbspink\.com))(\/[^\/]+)?\/([^\/]+)\/kako\/\d+(\/\d+)?\/(\d+)).html\"{$ext_win_target}>h?t?tp:\/\/[^\/]+(\.2ch\.net|\.bbspink\.com)(\/[^\/]+)?\/[^\/]+\/kako\/\d+(\/\d+)?\/\d+.html<\/a>/", array($this, 'link2chkako_callback'), $msg);
		
		/*
		// �u���N���`�F�b�J
		if ($brocra_checker['use']) {
			$msg = preg_replace("/<a href=\"(s?https?:\/\/[-_.!~*'()a-zA-Z0-9;\/?:@&=+\$,%#]+)\"{$ext_win_target}>(s?h?t?tps?:\/\/[-_.!~*'()a-zA-Z0-9;\/?:@&=+\$,%#]+)<\/a>/","<a href=\"\\1\"{$ext_win_target}>\\2</a> [<a href=\"{$brocra_checker['url']}?{$brocra_checker['query']}=\\1\"{$ext_win_target}>�`�F�b�N</a>]", $msg);
		}
		*/
	
		/*
		// �摜URL�����N���T���l�C����
		if ($preview_thumbnail) {
			$msg = preg_replace_callback("/<a href=\"(s?https?:\/\/[-_.!~*'()a-zA-Z0-9;\/?:@&=+\$,%#]+\.([jJ][pP][eE]?[gG]|[gG][iI][fF]|[pP][nN][gG]))\"{$ext_win_target}>(s?h?t?tps?:\/\/[-_.!~*'()a-zA-Z0-9;\/?:@&=+\$,%#]+\.([jJ][pP][eE]?[gG]|[gG][iI][fF]|[pP][nN][gG]))<\/a>/", array($this, 'view_img_callback') ,$msg);
		}
		*/
		
		// �� ime
		$msg = preg_replace_callback("/<a href=\"(s?https?:\/\/[-_.!~*'()a-zA-Z0-9;\/?:@&=+\$,%#]+)\"{$ext_win_target}>([^><]+)<\/a>/", array($this, 'ime_callback'), $msg);

		return $msg;
	}

	//=============================================================
	//�R�[���o�b�N���\�b�h
	//=============================================================

	/**
	 * ime_callback
	 */
	function ime_callback($s)
	{
		global $_conf, $ext_win_target;
		
		$r = '<a href="' . P2Util::throughIme($s[1]) . '"' . $ext_win_target . '>' . $s[2] . '</a>';
		return $r;
	}
		
	// ���p���X�ϊ� ===========================
	function quote_res_callback($s)
	{
		$rs = preg_replace_callback("/(&gt;|��)?(&gt;|��)?([0-9-]+)/", array($this, 'quote_res_devide_callback'), $s[0]);
		return $rs;
	}
	
	function quote_res_devide_callback($s)
	{
		global $_conf;
		global $k_at_a, $k_at_q, $k_input_ht;
		
		$appointed_num=$s[3];
		$qsign="$s[1]$s[2]";
		
		if($appointed_num=="-"){
			return $s[0];
		}
		
		$read_url="{$_conf['read_php']}?host={$this->thread->host}&amp;bbs={$this->thread->bbs}&amp;key={$this->thread->key}&amp;ls={$appointed_num}{$k_at_a}";

		$qnum=$appointed_num+0;
		if($qnum > sizeof($this->thread->datlines)){ //�����߂��郌�X�͕ϊ����Ȃ�
			return $s[0];
		}
		$rs=<<<EOP
<a href="{$read_url}">{$qsign}{$appointed_num}</a>
EOP;
		return $rs;
	}
	
	//2ch bbspink �������N===========================
	function link2ch_callback($s)
	{
		global $_conf;
		global $k_at_a, $k_at_q, $k_input_ht;
		
		$read_url="{$_conf['read_php']}?host=$s[1]&amp;bbs=$s[3]&amp;key=$s[4]&amp;ls=$s[6]{$k_at_a}";

		$rs=<<<EORS
		<a href="{$read_url}">$s[7]</a>
EORS;

		return $rs;
	}
	
	// �܂�BBS / JBBS���������  �������N===========================
	function linkMachi_callback($s)
	{
		global $_conf;
		global $k_at_a, $k_at_q, $k_input_ht;
	
	 	return "<a href=\"{$_conf['read_php']}?host={$s[1]}&amp;bbs={$s[4]}&amp;key={$s[6]}&amp;ls={$s[9]}-{$s[12]}{$k_at_a}\">{$s[13]}</a>";
	 }
	 
	// JBBS���������  �������N 2===========================
	function linkJBBS_callback($s)
	{
		global $_conf, $bbs_win_target;
	
	 	return "<a href=\"{$_conf['read_php']}?host={$s[1]}/{$s[2]}&amp;bbs={$s[3]}&amp;key={$s[4]}&amp;ls={$s[5]}\"{$bbs_win_target}>{$s[8]}</a>";
	}
	 
	//2ch�ߋ����Ohtml=============================
	function link2chkako_callback($s)
	{
		global $_conf;
		global $k_at_a, $k_at_q, $k_input_ht;
		/*
		$msg=preg_replace_callback("/<a href=\"(http:\/\/([^\/]+(\.2ch\.net|\.bbspink\.com))(\/[^\/]+)?\/([^\/]+)\/kako\/\d+(\/\d+)?\/(\d+)).html\"{$ext_win_target}>h?t?tp:\/\/[^\/]+(\.2ch\.net|\.bbspink\.com)(\/[^\/]+)?\/[^\/]+\/kako\/\d+(\/\d+)?\/\d+.html<\/a>/", array($this, 'link2chkako_callback'), $msg);
		*/
		$kakolog_uri = $s[1];
		$kakolog_uri_en = urlencode($kakolog_uri);
		$host=$s[2]; $bbs=$s[5]; $key=$s[7];
		$read_url="{$_conf['read_php']}?host={$host}&amp;bbs={$bbs}&amp;key={$key}&amp;kakolog={$kakolog_uri_en}{$k_at_a}";

			$rs=<<<EOP
<a href="{$read_url}">{$kakolog_uri}.html</a>
EOP;

		return $rs;
	}
	
	//�摜�|�b�v�A�b�v�ϊ�==========================
	function view_img_callback($s){
		global $pre_thumb_height, $pre_thumb_width;
	
		$img_tag=<<<EOIMG
<img class="thumbnail" src="$s[1]" height="{$pre_thumb_height}" weight="{$pre_thumb_width}" hspace="4" vspace="4" align="middle">
EOIMG;

		$rs =<<<EORS
			<a href="$s[1]">{$img_tag}{$s[3]}</a>
EORS;
		
		return $rs;
	}


}
?>