<?php
// p2 - BrdCtl -- �{�[�h���X�g�R���g���[���N���X for menu.php

require_once("./filectl_class.inc");
require_once("./datactl.inc");
require_once("./brdmenu_class.inc");


/**
* ���X�g �R���g���[�� �N���X
*/
class BrdCtl{
	
	/**
	* board��S�ēǂݍ���
	*/
	function read_brds()
	{
		$brd_menus_dir = BrdCtl::read_brd_dir();
		$brd_menus_online = BrdCtl::read_brd_online();
		$brd_menus = array_merge($brd_menus_dir, $brd_menus_online);
		return $brd_menus;
	}
	
	/**
	* board�f�B���N�g���𑖍����ēǂݍ���
	*/
	function read_brd_dir()
	{
		global $_info_msg_ht;
	
		$brd_menus = array();
		$brd_dir = "./board";
		
		if ($cdir = @dir($brd_dir)) {
			while( $entry = $cdir->read() ) {//�f�B���N�g������
				if($entry == "." || $entry == ".." || $entry == ".DS_Store"){ continue; }
				$filepath = $brd_dir."/".$entry;
				if( $data = @file($filepath) ){
					$aBrdMenu = new BrdMenu; //�N���X BrdMenu �̃I�u�W�F�N�g�𐶐�
					$aBrdMenu->setBrdMatch($filepath); //�p�^�[���}�b�`�`����o�^
					$aBrdMenu->setBrdList($data); //�J�e�S���[�Ɣ��Z�b�g
					$brd_menus[] = $aBrdMenu;
					unset($data, $aBrdMenu);
				}else{
					$_info_msg_ht .= "<p>p2 �G���[: ���X�g {$entry} ���ǂݍ��߂܂���ł����B</p>\n";
				}
			}
			$cdir->close();
		}
		
		return $brd_menus;
	}
	
	/**
	* �I�����C�����X�g��Ǎ���
	*/
	function read_brd_online()
	{
		global $_info_msg_ht, $brdfile_online, $c_menu_dl_interval;
		
		$brd_menus = array();
		
		if ($brdfile_online) {
			$cachefile = cacheForDL($brdfile_online);
			
			$noDL = false;
			if(  file_exists($cachefile.".p2.brd") ){	//�L���b�V��������ꍇ
				if($_GET['nr']){	// norefresh�Ȃ�DL���Ȃ�
					$noDL = true;
				}elseif( @filemtime($cachefile.".p2.brd") > time() - 60*60*$c_menu_dl_interval){	//�L���b�V���̍X�V���w�莞�Ԉȓ��Ȃ�DL���Ȃ�
					$noDL = true;
				}
			}
			
			if( $noDL  ){	// DL���Ȃ�
				;
			}else{	// DL����
				//echo "DL!<br>";//
				$brdfile_online_res = fileDownload($brdfile_online, $cachefile);
				if( $brdfile_online_res->is_success() && $brdfile_online_res->code!="304" ){
					$isNewDL = true;
				}
			}
		
			if( preg_match("/html?$/", $brdfile_online) ){ //html�`���Ȃ�
			
				//�X�V����Ă�����V�K�L���b�V���쐬
				if( $isNewDL ){
					//echo "NEW!<br>";//
					$aBrdMenu = new BrdMenu; //�N���X BrdMenu �̃I�u�W�F�N�g�𐶐�
					$aBrdMenu->makeBrdFile($cachefile); //.p2.brd�t�@�C���𐶐�
					$brd_menus[] = $aBrdMenu;
					$read_html_flag = true;
					unset($aBrdMenu);
				}
				
				if( file_exists($cachefile.".p2.brd") ){
					$cashe_brd = $cachefile.".p2.brd";
				}else{
					$cashe_brd = $cachefile;
				}
				
			}else{
				$cashe_brd = $cachefile;
			}
			
			if(! $read_html_flag){
				if( $data = @file($cashe_brd) ){
					$aBrdMenu = new BrdMenu; //�N���X BrdMenu �̃I�u�W�F�N�g�𐶐�
					$aBrdMenu->setBrdMatch($cashe_brd); //�p�^�[���}�b�`�`����o�^
					$aBrdMenu->setBrdList($data); //�J�e�S���[�Ɣ��Z�b�g
					if($aBrdMenu->num){
						$brd_menus[] = $aBrdMenu;
					}else{
						$_info_msg_ht .=  "<p>p2 �G���[: {$cashe_brd} ������j���[�𐶐����邱�Ƃ͂ł��܂���ł����B</p>\n";
					}
					unset($data, $aBrdMenu);
				}else{
					$_info_msg_ht .=  "<p>p2 �G���[: {$cachefile} �͓ǂݍ��߂܂���ł����B</p>\n";
				}
			}
		}
		
		return $brd_menus;
	}

}
?>