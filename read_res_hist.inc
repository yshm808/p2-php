<?php
// p2 - �������ݗ��� �̂��߂̊֐��Q

//======================================================================
// �֐�
//======================================================================
/**
 * �`�F�b�N�����������݋L�����폜����֐�
 */
function deleMsg($checked_hists)
{
	global $p2_res_hist_dat_php;

	// �ǂݍ����
	if (!$reslines = P2Util::fileDataPhp($p2_res_hist_dat_php)) {
		die("p2 Error: {$p2_res_hist_dat_php} ���J���܂���ł���");
	}

	// �`�F�b�N���Đ�����
	$neolines = "";
	if ($reslines) {
		$n = 1;
		foreach ($reslines as $ares) {
			// &<>/ �� &xxx; �̃G�X�P�[�v�����ɖ߂�
			$ares = P2Util::unescapeDataPhp($ares);
			
			$rar = explode("\t", $ares);
			
			// �ԍ��Ɠ��t����v���邩���`�F�b�N����
			if (checkMsgID($checked_hists, $n, $rar[2])) {
				$rmnums[] = $n; // �폜����ԍ���o�^
			}
			
			$n++;
		}
		$neolines = rmLine($rmnums, $reslines);
	}

	// ��������
	$fp = @fopen($p2_res_hist_dat_php, "wb") or die("p2 Error: {$p2_res_hist_dat_php} ���X�V�ł��܂���ł���");
	if ($neolines) {
		// �擪�s��ǉ�
		fputs($fp, "<?php /*\n");
		foreach ($neolines as $l) {			
			// �f�[�^�s���L�^
			fputs($fp, $l);
		}
		// ���s��ǉ�
		fputs($fp, "*/ ?>\n");
	}
	fclose($fp);
}

/**
 * �ԍ��Ɠ��t����v���邩���`�F�b�N����֐�
 */
function checkMsgID($checked_hists, $order, $date)
{
	if ($checked_hists) {
		foreach ($checked_hists as $v) {
			$vary = explode(",,,,", $v);	// ",,,," �͊O�����痈��ϐ��ŁA����ȃf���~�^
			//echo "$vary[0]==$order,$vary[1]==$date<br>";
			if (($vary[0]==$order) and ($vary[1]==$date)) {
				return true;
			}
		}
	}
	return false;
}

/**
 * �w�肵���ԍ��i�z��w��j���s���X�g����폜����֐�
 */
function rmLine($order_list, $lines)
{
	if ($lines) {
		$i = 0;
		foreach ($lines as $l) {
			$i++;
			if( checkOrder($order_list, $i) ){ continue; } // �폜����
			$neolines[] = $l;
		}
		return $neolines;
	}
	return false;
}

/**
 * �ԍ��Ɣz����r
 */
function checkOrder($order_list, $order)
{
	if ($order_list) {
		foreach ($order_list as $n) {
			if ($n==$order) {
				return true;
			}
		}
	}
	return false;
}

?>