<?php
/*... 
Этим файлом, я 13.09.16 начинаю работу по изготовлению программы поиска подходящего сетевого оборудования фирмы HPE

readFiles( $nameFile) //.. Читаем содержимое файла в $Pieces, а описание в $Parametrs
multOut( $netData ) //.. Размножаем "Серию" по всем продуктам
tableOut ( $top, $netData ) //.. Вывод в табличной форме информации о продуктах
collectParameters ( $Description, $Products ) //.. Выбираем поисковые параметры для последующего поиска
zaprosParameters( $pLevel, $pNumPorts, $pPOEbudget, $pMAC, $pProizv, $pPropusk) //.. Вывод на экран страницы с параметрами поиска
packToChoice ( $pLevel) //.. Формируем структурку выпадающего меню
searchFor( $choices ) //.. Основная поисковая процедура. На выходе массив удовлетворяющий критериям поиска
compareFor( $choices ) //.. Построчное сравнение
searchOut( $top, $mSearch, $choices ) //.. Вывод результатов, аналогично tableOut
---------- Сессии для передачи переменных между программами
setSession ( $Description, $Products ) //.. Заносим в сессию Массив Описаний параметров и Описание продуктов
getSession ( ) //.. Получить данные из Сессии 
-----------------------------------------------------------
$mH - Должно быть
byPortsOnly ( $Сhoices[0], $iSumС ) //.. Нашли или нет нужное количество портов
bySpeedOnly ( $Сhoices[1], $s100, $s1g, $s10g, $ssfp, $ssfpp, $sqsfp ) //.. Только по скорости портов
bySredaOnly ( $Сhoices[2], $cu, $fb ) //.. Среда
byPoeBudgetOnly ( $Сhoices[4], $hpoew ) //.. PoE Бюджет
byMACOnly ( $Сhoices[6], $hmac )

byPortsSpeed ( $Сhoices, $s100, $s1g, $s10g, $ssfp, $ssfpp, $sqsfp ) //.. По параметрам Количество определенных по скорости портов
byPortsSpeedTest( $begin, $end, $Сhoices, $s100, $s1g, $s10g, $ssfp, $ssfpp, $sqsfp ) //.. В помощь byPortsSpeed
-----------------------------------------------------------

*/

//.....................................................................................................................
//..
session_start();
error_reporting( E_ALL );

function readFiles( $nameFile )
{

	//... TEST: print "Begin:: readFiles()";

	$List = file ( $nameFile );
	$iList = count ( $List );
	
	if ( $iList == 0 ) die ("$iList is empty");
		
	$Parametrs = explode ("~", $List[ 0 ]); // Расчленяем нулевую запись на описания!
	$iParametrs = count ( $Parametrs );
	
	//.. Test: print "В файле $nameFile всего $iList записей \\ В описании $iParametr полей\n\c";

	//$iRealSring = 0;
	//...А теперь полный вперед!

	for ( $j=1; $j < $iList; $j++){		
		$Pieces = explode ("~", $List[ $j ]); // Расчленяем
		//...$iPieces = count ($Pieces);
		if ( strlen($Pieces[0])<=1 && strlen($Pieces[1])<=1 ) continue; //.. Пропускаем не информативные ( пустые ) строки
		//$iRealSring++;
		//.. Test:: print "$j -- $iRealSring\n<br>";
		$massOut[] = $Pieces;
	}

	//... TEST: print "End:: readFile()";

return array( $Parametrs, $massOut );
}

//.....................................................................................................................
//..
function multOut( $netData )
{
	$iData = count ($netData);
	$tmpSeries = $netData[0][0];

	//$ii = 1;
	//print "$ii==$tmpSeries<br>";
	
	for ( $j=1; $j < $iData; $j++){
		if ( strlen ($netData[$j][0]) > 1 ){
				$tmpSeries = $netData[$j][0];
				//$ii++;
				//print "$ii=$tmpSeries <br>";
			} else {
			$netData[$j][0] = $tmpSeries;
		}
	}
return ( $netData );
}

//.....................................................................................................................
//..
function tableOut( $top, $netData )
{

//.....................................................
//.. Номер и имя
	$name = 1;
	$part = 2;
//.. Блок портов
	$c100 = 7;
	$p100 = 8;
	$pp100 = 9;
	$c1k = 10;
	$p1k = 12;
	$pp1k = 13;
	$c10k = 11;
//.. Блок СФП
	$sfp = 14;
	$sfpdp = 15;
	$sfpp = 16;
	$qsfp = 17;
//.. Блок производительность
	$ps = 20;
	$pk = 21;
	$mac = 22;
	$poew = 25;
//..Номер столбца в котором находится имя файла Серии
	$iImage = 29;
//.....................................................

//.. Вывод заголовка таблицы
$textOut = <<<OUTs
<table style="padding: 10px 0 10px; border-radius: 10px; background-color: #F0F8FF; border: 1px dotted #C4C9D1; color: #0b3d91;">
<tr style="background: #ace7f2;">
	<td>$top[$part]</td>
	<td>$top[$name]</td>
	<td>$top[$c100]</td>
	<td>$top[$p100]</td>
	<td>$top[$pp100]</td>
	<td>$top[$c1k]</td>
	<td>$top[$p1k]</td>
	<td>$top[$pp1k]</td>
	<td>$top[$c10k]</td>
	<td>$top[$sfp]</td>
	<td>$top[$sfpp]</td>
	<td>$top[$qsfp]</td>	
	<td>$top[$ps]</td>
	<td>$top[$pk]</td>
	<td>$top[$mac]</td>
	<td>$top[$poew]</td>
</tr>
OUTs;
print $textOut;

//.. Тело таблицы
	$iData = count ($netData);
	$Series = $netData[0][0];
	$ImageSeries = $netData[0][$iImage];

	//..TEST::print "<br>ImageSeries =$ImageSeries<br>";
	
if ( strlen ($ImageSeries) > 1 && file_exists("image/".$ImageSeries) ) {
	
$textOut = <<<OUTs
<tr>
	<td colspan="16" style="text-align: center; border-top: 1px solid; border-bottom: 1px dotted;"><img src="image/$ImageSeries" alt="$Series" border="0" height="100" style="vertical-align: middle;" /><b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$Series</b></td>
</tr>
OUTs;
	} else {
$textOut = <<<OUTs
<tr height="50">
	<td colspan="16" align="center" style="text-align: center; border-top: 1px solid; border-bottom: 1px dotted;"><b>$Series</b></td>
</tr>
OUTs;
	}
print $textOut;

	for ( $j=0, $k=0; $j < $iData; $j++){
		
		//.. Проверяем изменилась ли "серия" коммутаторов или нет
		if ( strcmp ($Series, $netData[$j][0]) ){
			
			$Series = $netData[$j][0];
			$ImageSeries = $netData[$j][$iImage];
			$k=0; //... Это для переключения цвета полоски!
			
			//.. Проверяем есть ли изображение к данной серии
			if ( strlen ($ImageSeries) > 1 && file_exists("image/".$ImageSeries) ) { 
					//.. Если изображение есть, то мы перед именем серии выводим изображение
					//.. 22/09 - Нет, так не пойдет. Для вывода картинки нужно чтобы она еще и на диске была. Это надо проверить!!!!

$textOut = <<<OUTs
<tr>
	<td colspan="16" align="center" style="border-top: 1px solid #C4C9D1; border-bottom: 1px dotted;"><img src="image/$ImageSeries" alt="$Series" border="0" height="100" style="vertical-align: middle;" /><b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$Series</b></td>
</tr>
OUTs;
			
				} else { //.. Иначе выводим только название Серии
			
$textOut = <<<OUTs
<tr height="50">
	<td colspan="16" align="center" style="border-top: 1px solid #C4C9D1; border-bottom: 1px dotted;"><b>$Series</b></td>
</tr>
OUTs;
			}

print $textOut;		
		
		} //.. Окончание блока, который выполняется когда изменяется Серия
		
		
		//.. Определяем цвет выводимой полоски ( по переменной $k)
		if ( $k%2 ) $tr = "<tr align=\"center\">"; 
			else 
			$tr = "<tr align=\"center\" style=\"color: #0b3d91; background: #f0ffff;\">";
		$k++;
		
//.. Блок вывода строки данных по модели	
		$i1 = $netData[$j][$part];
		$i2 = $netData[$j][$name];
		$i3 = $netData[$j][$c100];
		$i4 = $netData[$j][$p100];
		$i5 = $netData[$j][$pp100];
		$i6 = $netData[$j][$c1k];
		$i7 = $netData[$j][$p1k];
		$i8 = $netData[$j][$pp1k];
		$i9 = $netData[$j][$c10k];
		$i13 = $netData[$j][$sfp];
		$i14 = $netData[$j][$sfpp];
		$i15 = $netData[$j][$qsfp];		
		$i10 = $netData[$j][$ps];
		$i11 = $netData[$j][$pk];
		$i12 = $netData[$j][$mac];
		$i16 = $netData[$j][$poew];
		
$textOut = <<<OUTs
$tr
	<td>$i1</td>
	<td align="left">$i2</td>
	<td>$i3</td>
	<td>$i4</td>
	<td>$i5</td>		
	<td>$i6</td>
	<td>$i7</td>
	<td>$i8</td>
	<td>$i9</td>
	<td>$i13</td>
	<td>$i14</td>
	<td>$i15</td>	
	<td>$i10</td>
	<td>$i11</td>	
	<td>$i12</td>	
	<td>$i16</td>	
</tr>
OUTs;
print $textOut;
	
	}
	
//.. Финал таблицы
print "<table>";

}

//.. Выбираем поисковые параметры для последующего поиска ( что то мне кажется это только на время разработки программы)

function collectParameters ( )
{
//.. Итак, какие параметры мы выбираем 
//--------------------------------------------------------------------------------------------
//.. +Коммутаторы Уровня ( 2; 3 Lite; 3 Dynamic; 3 Advanced )
//.. Количество портов ( 6 групп: 5портов; 8-10; 16-20; 24-28; 48-52; больше 52 )
//.. Коммутаторы ( 10/100 ; 100/1K; 10K
//.. Коммутаторы с PoE
//.. Коммутаторы с PoE+
//.. +PoE бюджет ( до 124W; до 382W; свыше 382W)
//.. +Коммутаторы с таблицей MAC (не менее: 8192; 16384; до 32768 )
//.. +Коммутаторы с производительностью ( не менее: 20; 80; 200; 550; 1000; 2000)
//.. +Коммутаторы с пропускной способностью ( не менее: 10; 50; 100; 250)
//.. Коммутаторы без вентиляторов fanless

	$pNumPorts = array( "5", "8-10", "16-20", "24-28","48-52", "больше 52");
	$pPorts = array( "10/100", "100/1K", "1K/10K", "40K");
	$pSreda = array( "витая пара", "оптика");
	$pPOEbudget = array( "до 124W", "до 382W", "выше 382W");
	$pLevel = array( "2", "3 Lite", "3 Dynamic", "3 Advanced");
	$pMAC = array( "до 8192", "до 16384", "до 32768");
	$pProizv = array( "до 20", "от 20 до 80", "от 80 до 200", "от 200 до 550", "от 550 до 1000", "от 1000 до 2000");
	$pPropusk = array( "до 10", "до 50", "до 100", "до 250");

return array ( $pLevel, $pNumPorts, $pSreda, $pPorts, $pPOEbudget, $pMAC, $pProizv, $pPropusk) ;

}

function zaprosParameters( $pLevel, $pNumPorts, $pSreda, $pPorts, $pPOEbudget, $pMAC, $pProizv, $pPropusk )
{

$i1 = packToChoice ( $pNumPorts, 0);
$i2 = packToChoice ( $pPorts, 1);
$i21 = packToChoice ( $pSreda, 2);
$i3 = packToChoice ( array("PoE/PoE+"), 3);
$i4 = packToChoice ( $pPOEbudget, 4 );
$i5 = packToChoice ( $pLevel, 5);
$i6 = packToChoice ( $pProizv, 6);
$i7 = packToChoice ( $pPropusk, 7);
$i8 = packToChoice ( $pMAC, 8);

$textOut = <<<OUTs
<form action="zapros.php" method="POST" name="Solution">
<table  align="center" style="width: 60%; padding: 10px 0 10px; background-color: #F0F8FF; border-radius: 10px; border: 1px dotted #C4C9D1; color: #0b3d91;">
				<tr>
								<td colspan="4" align="center" style="border-bottom: 1px dotted #C4C9D1;">
								<h2>Подбор сетевого оборудования HPE <br>по основным параметрам</h2>
								</td>
				</tr>
				<tr>
								<td>&nbsp;</td>
								<td colspan="3">&nbsp;</td>
				</tr>
				<tr>
								<td>Количество портов</td>
								<td colspan="3">$i1</td>
				</tr>
				<tr>
								<td>Максимальная скорость передачи данных</td>
								<td>$i2</td>
								<td>Среда передачи данных</td>
								<td>$i21</td>
				</tr>
				<tr>
								<td>Нужны ли порты PoE/PoE+</td>
								<td>$i3</td>
								<td>Выберите PoE бюджет</td>
								<td>$i4</td>
				</tr>
				<tr height="50">
								<td colspan="4" align="center">
								<p>&nbsp;
								<h3>Выберите дополнительные технические характеристики</h3>
								</td>
				</tr>
				<tr>
								<td>Уровень коммутации</td>
								<td>Производительность (Гбит/с)</td>
								<td>Пропускная способность (млн пакетов/с)</td>
								<td>Таблица маршрутизации</td>
				</tr>
				<tr height="50">
								<td align="center">$i5</td>
								<td align="center">$i6</td>
								<td align="center">$i7</td>
								<td align="center">$i8</td>
				</tr>
				<tr height="50">
								<td colspan="4" align="center" style="border-top: 1px dotted #C4C9D1;">
								<p>&nbsp;
								<p>
								<input type=hidden name="subject" value="подбор сетевого оборудования по параметрам">
								<!-- <input type=hidden name=choices value=sz> -->
								<input name="reset" type="reset" value="Сбросить"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
								<input name="submit" type="submit" value="Подобрать!">
								</td>
				</tr>
</table>
</form>
OUTs;
print $textOut;

}


function packToChoice ( $param, $i)
{
	
	$str = count ( $param );
	$select_param = "\n<select name='sz[$i]'>\n";
	$select_param .= "<option value='0' selected>не важно</option>\n";
	
	for ($i=0, $j=1; $i < $str; $i++, $j++ ) $select_param .= "<option value='$j'>$param[$i]</option>\n";

	$select_param .= "</select>";
	
return $select_param;
}

//.....................................................................................................................
//..
function searchFor( $choices, $netData )
{

	//.. TEST:: print "enter to searchFor()<br>\n"; print_r ($choices);
	
	$iData = count ($netData);
	//.. TEST:: print "<br>iData=$iData<br>\n";
	
//.. Идем по основному массиву и построчно проверяем на удовлетворения параметрам $choices
//.. Тело таблицы

	for ( $j=0; $j < $iData; $j++){ 
		if ( compareFor( $choices, $netData[$j] )) {
			//.. TEST:: print "После compareFor()\n"; print_r ($netData[$j]);
			$mSearch[] = $netData[$j];
		}
	}
	if ( !isset($mSearch)) $mSearch[0]=0;
	return ( $mSearch );
}
//.. Логика такова... Сначала проходим по всем запрошенным характеристикам и собирам по ним сведения
//.. После сбора данных анализируем соответствует или нет срока запросу
function compareFor( $Сhoices, $netDataString )
{
//.....................................................
//.. Номер и имя, уровень
	$name = 1;
	$part = 2;
	$iLevel = 5;
//.. Блок портов
	$c100 = 7;
	$p100 = 8;
	$pp100 = 9;
	$c1k = 10;
	$p1k = 12;
	$pp1k = 13;
	$c10k = 11;
//.. Блок СФП
	$sfp = 14;
	$sfpdp = 15;
	$sfpp = 16;
	$qsfp = 17;
//.. Блок производительность
	$ps = 20;
	$pk = 21;
	$mac = 22;
	$poew = 25;
//..Номер столбца в котором находится имя файла Серии
	$iImage = 29;
//.....................................................
//.. TEST:: print_r $netDataString; print "Вошли в compareFor()\n"; 
	
//.. А вот так если... беру цикл, в нем про свичу проверяю подходит или нет и если все проходит, то Гуд, нет - Бэд

	$iСhoices = count ($Сhoices);
	$iResult = 0; //.. Отрицательный итог
//.. TEST:: print "<br>iСhoices=$iСhoices<br>";	print "--before cycle iResult=$iResult<br>";
	
	for ( $j = 0; $j < $iСhoices; $j++) {
		//..  print "--cycle $j";
		//.. Это суммы по портам Медь и Оптика
		$iSumС = $netDataString[$c100]+$netDataString[$p100]+$netDataString[$pp100]+$netDataString[$c1k]+$netDataString[$c10k]+$netDataString[$p1k]+$netDataString[$pp1k];
		$iSumO = $netDataString[$sfp]+$netDataString[$sfpdp]+$netDataString[$sfpp]+$netDataString[$qsfp];
		switch ( $j ) {
			case 0://.. Считаем количество портов ( медь и оптика )
				$mH0 = $Сhoices[0];
				//..TEST:: print "<br> iSum=$iSum<br>iSumO=$iSumO<br>";
				break; 
			case 1://.. Считаем порты по "скоростям"
				$mH1 = $Сhoices[1];
				$s100 = (isset ( $netDataString[$c100] ) or isset ( $netDataString[$p100] ) or isset ( $netDataString[$pp100] ))? $netDataString[$c100]+$netDataString[$p100]+$netDataString[$pp100] : 0;
				$s1g = (isset ( $netDataString[$c1k] ) or isset ( $netDataString[$p1k] ) or isset ( $netDataString[$pp1k] ))? $netDataString[$c1k]+$netDataString[$p1k]+$netDataString[$pp1k] : 0;
				$s10g = (isset ( $netDataString[$c10k] ) and strlen($netDataString[$c10k]) > 0)? $netDataString[$c10k]:0;
				$ssfp = (isset ( $netDataString[$sfp] ) or isset ( $netDataString[$sfpdp] ))? $netDataString[$sfp]+$netDataString[$sfpdp] : 0;
				$ssfpp = (isset ( $netDataString[$sfpp] ) and strlen($netDataString[$sfpp]) > 0)? $netDataString[$sfpp] : 0;
				$sqsfp = (isset ( $netDataString[$qsfp] ) and strlen($netDataString[$qsfp]) > 0)? $netDataString[$qsfp] : 0;
				break;
			case 2://.. Считаем порты по среде передачи данных
				$mH2 = $Сhoices[2];
				$cu = ((isset ( $netDataString[$c100] ) or isset ( $netDataString[$p100] ) or isset ( $netDataString[$pp100] )
						or isset ( $netDataString[$c1k] ) or isset ( $netDataString[$p1k] ) or isset ( $netDataString[$pp1k] )
						or isset ( $netDataString[$c10k] )) and
						(strlen($netDataString[$c100] )> 0 or strlen( $netDataString[$p100] )> 0 or strlen( $netDataString[$pp100] )> 0
						or strlen( $netDataString[$c1k] )> 0 or strlen( $netDataString[$p1k] )> 0 or strlen( $netDataString[$pp1k] )> 0
						or strlen( $netDataString[$c10k] )> 0))? 1: 0;
				$fb = ((isset ( $netDataString[$sfp] ) or isset ( $netDataString[$sfpdp] ) 
						or isset ( $netDataString[$sfpp] ) or isset ( $netDataString[$qsfp] )) and
						((strlen( $netDataString[$sfp] )> 0 or strlen( $netDataString[$sfpdp] )> 0 
						or strlen( $netDataString[$sfpp] )> 0 or strlen( $netDataString[$qsfp] )> 0)))? 1: 0;
						
				//.. TEST:: $tName=$netDataString[$part]; print "$tName=$cu=$fb=<br>";
				break;
			case 3://.. Нужно PoE?
				$mH3 = $Сhoices[3];
				$poe = ( (isset ( $netDataString[$pp100]) or isset ( $netDataString[$pp1k] )) and 
						(strlen( $netDataString[$pp100])>0 or strlen( $netDataString[$pp1k] )>0) )? 1:0;
				break;
			case 4://.. Какой PoE бюджет?
				$mH4 = $Сhoices[4];
				$hpoew = ( isset ( $netDataString[$poew]) )? $netDataString[$poew]: 0;
				break;
			case 5://.. Уровень коммутации
				$mH5 = $Сhoices[5];
				$level = (isset ( $netDataString[$iLevel] ) and strlen ($netDataString[$iLevel])>0 )? $netDataString[$iLevel]:0;
				break;
			case 6://.. Производительность
				$mH7 = $Сhoices[7];
				$hpk = (isset ( $netDataString[$pk] ))? $netDataString[$pk]:0;
				break;
			case 7://.. Пропускная способность
				$mH8 = $Сhoices[8];
				$hps = (isset ( $netDataString[$ps] ))? $netDataString[$ps]:0;
				break;
			case 8://.. Таблица МАС
				$mH6 = $Сhoices[6];
				$hmac = (isset ( $netDataString[$mac] ) and strlen ($netDataString[$mac])>0 )? $netDataString[$mac]:0;
				break;
		}
	}
//.......................................................................................
//.......................................................................................
//.. Аналитическая часть.
//..	Ничего не выбрано
		if ( !($mH0 or $mH1 or $mH2 or $mH3 or $mH4 or $mH5 or $mH6 or $mH7 or $mH8)){
			return ($iResult = 1); //..... Выводим ВСЁ!
		}
//.. Выбран только один параметр
//.. Количество портов
		if ( $mH0 and (!( $mH1 or $mH2 or $mH3 or $mH4 or $mH5 or $mH6 or $mH7 or $mH8)) ) 
				if ( !$iResult = byPortsOnly ( $Сhoices[0], $iSumС, $iSumO )) return (0);
//.. Скорость портов
		if ( $mH1 and (!( $mH0 or $mH2 or $mH3 or $mH4 or $mH5 or $mH6 or $mH7 or $mH8)) ) 
				if ( !$iResult = bySpeedOnly ( $Сhoices[1], $s100, $s1g, $s10g, $ssfp, $ssfpp, $sqsfp )) return (0);
//.. Среда передачи данных
		if ( $mH2 and (!( $mH0 or $mH1 or $mH3 or $mH4 or $mH5 or $mH6 or $mH7 or $mH8)) ) 
				if ( !$iResult = bySredaOnly ( $Сhoices[2], $cu, $fb )) return (0);
//.. PoE
		if ( $mH3 and (!( $mH0 or $mH1 or $mH2 or $mH4 or $mH5 or $mH6 or $mH7 or $mH8)) ) 
				if ( !$iResult = $poe) return (0);
//.. бюджет PoE
		if ( $mH4 and (!( $mH0 or $mH1 or $mH2 or $mH3 or $mH5 or $mH6 or $mH7 or $mH8)) ) 
				if ( !$iResult = byPoeBudgetOnly ( $Сhoices[4], $hpoew )) return (0);
//.. Уровень коммутации
		if ( $mH5 and (!( $mH0 or $mH1 or $mH2 or $mH3 or $mH4 or $mH6 or $mH7 or $mH8)) ) 
				if ( !$iResult = byLevelOnly ( $Сhoices[5], $level )) return (0);
//.. Производительность
		if ( $mH6 and (!( $mH0 or $mH1 or $mH2 or $mH3 or $mH4 or $mH5 or $mH7 or $mH8)) ) 
				if ( !$iResult = byPkOnly ( $Сhoices[6], $hpk )) return (0);
//.. Пропускная способность
		if ( $mH7 and (!( $mH0 or $mH1 or $mH2 or $mH3 or $mH4 or $mH5 or $mH6 or $mH8)) ) 
				if ( !$iResult = byPsOnly ( $Сhoices[7], $hps )) return (0);
//.. Таблица коммутации
		if ( $mH8 and (!( $mH0 or $mH1 or $mH2 or $mH3 or $mH4 or $mH5 or $mH6 or $mH7)) ) 
				if ( !$iResult = byMACOnly ( $Сhoices[8], $hmac )) return (0);
//.......................................................................................
//.. Если выбрано 2 параметра из первых СВОДНЫХ ( тех которые нужно обязательно считать)
//.. Количество портов и скорость на портах
		if ( $mH0 and $mH1 and (!($mH2 or $mH3 or $mH4)) ) 
				if ( !$iResult = byPortsSpeed ( $Сhoices, $s100, $s1g, $s10g, $ssfp, $ssfpp, $sqsfp )) return (0);
//.......................................................................................
//.. Количество портов и среда передачи
		if ( $mH0 and $mH2 and (!($mH1 or $mH3 or $mH4)) ) 
				if ( !$iResult = byPortsSreda ( $Сhoices, $iSumС, $iSumO )) return (0);
//.......................................................................................
//.......................................................................................
//.. мы проверяем по одному ИЗ параметров и получается что мы смотрим если их несколько...
//.. По количеству портов соответствует?
//.. TEST
		$iResult = 1;
		//..TEST:: print "$netDataString[2]>iSumС=$iSumС/iSumO=$iSumO/$s100, $s1g, $s10g, $ssfp, $ssfpp, $sqsfp/$cu, $fb/hpoew=$hpoew/$level=?$mH5/hpk=$hpk/hps=$hps/hmac=$hmac/<br>";
		if ( $iResult and $mH0 ) if( !byPortsOnly( $Сhoices[0], $iSumС, $iSumO ) ) return(0);
		//print "after 0<br>";
		if ( $iResult and $mH1 ) if( !bySpeedOnly ( $Сhoices[1], $s100, $s1g, $s10g, $ssfp, $ssfpp, $sqsfp ) ) return(0);
		//print "after 1<br>";
		if ( $iResult and $mH2 ) if( !bySredaOnly ( $Сhoices[2], $cu, $fb )) return(0);
		//print "after 2<br>";
		if ( $iResult and $mH3 ) if( !$poe ) return(0);
		//print "after 3<br>";
		if ( $iResult and $mH4 ) if( !byPoeBudgetOnly ( $Сhoices[4], $hpoew )) return(0);
		//print "after 4<br>";
		if ( $iResult and $mH5 ) if( !byLevelOnly ( $Сhoices[5], $level )) return(0);
		//print "after 5<br>";
		if ( $iResult and $mH6 ) if ( !byPkOnly ( $Сhoices[6], $hpk )) return(0);
		//print "after 6<br>";
		if ( $iResult and $mH7 ) if ( !byPsOnly ( $Сhoices[7], $hps )) return(0);
		//print "after 7<br>";
		if ( $iResult and $mH8 ) if ( !byMACOnly ( $Сhoices[8], $hmac )) return(0);
		//print "after 8<br>";

//.......................................................................................
//.......................................................................................
return ( $iResult );
}
//.......................................................................................

//...........................................................................................................................
//.. Функции определяющие удовлетворяем мы параметрам или нет...
//.. По количеству портов 0 - нет нужных портов: 1 - нашли!
function byPortsOnly ( $Сhoices, $iSumС, $iSumO )
{
		switch ( $Сhoices ){
			case 0: return (1); //.. Результат не важен или "нашли!"
			case 1: if ( $iSumС == 5 or $iSumO == 5) return (1);
				break;
			case 2: if ( ($iSumС >= 8 && $iSumС <=10) or ($iSumO >= 8 && $iSumO <=10)) return (1);
				break;
			case 3: if ( ($iSumС >= 16 && $iSumС <=20) or ($iSumO >= 16 && $iSumO <=20) ) return (1);
				break;
			case 4: if ( ($iSumС >= 24 && $iSumС <=28) or ($iSumO >= 24 && $iSumO <=28)) return (1);
				break;
			case 5: if ( ($iSumС >= 48 && $iSumС <=52) or ($iSumO >= 48 && $iSumO <=52)) return (1);
				break;
			case 6: if ( ($iSumС > 52) or ($iSumO > 52) ) return (1);
				break;
		}
return ( 0 );
}
//...........................................................................................................................
//.. По скорости портов 0 - нет нужных портов: 1 - нашли!
function bySpeedOnly ( $Сhoices, $s100, $s1g, $s10g, $ssfp, $ssfpp, $sqsfp )
{
		switch ( $Сhoices ){
			case 0: return (1); //.. НЕВЕРОЯТНО
			case 1: if ( $s100 > 0 ) return (1);
				break;
			case 2: if ( $s1g+$ssfp > 0 ) return (1);
				break;
			case 3: if ( $s10g+$ssfpp > 0 ) return (1);
				break;
			case 4: if ( $sqsfp > 0 ) return (1);
				break;
		}
return ( 0 );
}
//...........................................................................................................................
//.. По среде 0 - нет нужных портов: 1 - нашли!
function bySredaOnly ( $Сhoices, $cu, $fb )
{
		switch ( $Сhoices ){
			case 0: return (1); //.. НЕВЕРОЯТНО
			case 1: if ( $cu ) return (1);
				break;
			case 2: if ( $fb ) return (1);
				break;
		}
return ( 0 );
}
//...........................................................................................................................
//.. По PoE бюджету 0 - нет нужных портов: 1 - нашли!
function byPoeBudgetOnly ( $Сhoices, $hpoew )
{
		switch ( $Сhoices ){
			case 0: return (1); //.. НЕВЕРОЯТНО
			case 1: if ( $hpoew > 0 and $hpoew <= 124 ) return (1);
				break;
			case 2: if ( $hpoew > 124 and $hpoew <= 382 ) return (1);
				break;
			case 3: if ( $hpoew > 382 ) return (1);
				break;
		}
return ( 0 );
}
//...........................................................................................................................
function byLevelOnly ( $Сhoices, $level )
{
		switch ( $Сhoices ){
			case 0: return (1); //.. НЕВЕРОЯТНО
			case 1: if ( preg_match( "/2/i", $level) ) return (1);
				break;
			case 2: if ( preg_match( "/3 Lite/i", $level) ) return (1);
				break;
			case 3: if ( preg_match( "/3 Dynamic/i", $level) ) return (1);
				break;
			case 4: if ( preg_match( "/3 Advanced/i", $level) ) return (1);
				break;
		}
return ( 0 );
}
//...........................................................................................................................
//.. Производительность
function byPkOnly ( $Сhoices, $hpk )
{
//..TEST::print "Сhoices=$Сhoices//hpk=$hpk<br>";
		
		switch ( $Сhoices ){
			case 0: return (1); //.. НЕВЕРОЯТНО
			case 1: if ( $hpk > 0 and $hpk <= 20 ) return (1);
				break;
			case 2: if ( $hpk > 20 and $hpk <= 80 ) return (1);
				break;
			case 3: if ( $hpk > 80 and $hpk <= 200 ) return (1);
				break;
			case 4: if ( $hpk > 200 and $hpk <= 550 ) return (1);
				break;
			case 5: if ( $hpk > 550 and $hpk <= 1000 ) return (1);
				break;
			case 6: if ( $hpk > 1000 and $hpk <= 2000 ) return (1);
				break;
		}
return ( 0 );
}
//...........................................................................................................................
//.. Пропускная способность
function byPsOnly ( $Сhoices, $hps )
{
//..TEST::print "Сhoices=$Сhoices//hps=$hps<br>";
		
		switch ( $Сhoices ){
			case 0: return (1); //.. НЕВЕРОЯТНО
			case 1: if ( $hps > 0 and $hps <= 10 ) return (1);
				break;
			case 2: if ( $hps > 10 and $hps <= 50 ) return (1);
				break;
			case 3: if ( $hps > 50 and $hps <= 100 ) return (1);
				break;
			case 4: if ( $hps > 100 and $hps <= 250 ) return (1);
				break;
		}
return ( 0 );
}
//...........................................................................................................................
//.. Таблица МАС
function byMACOnly ( $Сhoices, $hmac )
{
//..TEST::print "hmac=$hmac<br>";
		
		switch ( $Сhoices ){
			case 0: return (1); //.. НЕВЕРОЯТНО
			case 1: if ( $hmac > 0 and $hmac <= 8192 ) return (1);
				break;
			case 2: if ( $hmac > 8192 and $hmac <= 16384 ) return (1);
				break;
			case 3: if ( $hmac > 16384 ) return (1); //32768
				break;
		}
return ( 0 );
}
//...........................................................................................................................
//...........................................................................................................................
//.. По количеству портов 0 - нет нужных портов: 1 - нашли!
function byPortsSpeed ( $Сhoices, $s100, $s1g, $s10g, $ssfp, $ssfpp, $sqsfp )
{
//.. ТЕСТ::print "\r\n1/$Сhoices[1]/ $s100, $s1g, $s10g, $ssfp, $ssfpp, $sqsfp /<br>";
		switch ( $Сhoices[0] ){
			case 0: return (1); //.. НЕВЕРОЯТНО!!!! 
			case 1:  return ( byPortsSpeedTest( 5,5, $Сhoices[1], $s100, $s1g, $s10g, $ssfp, $ssfpp, $sqsfp ) ); //.. Портов 5
				break;
			case 2: return ( byPortsSpeedTest( 8, 10, $Сhoices[1], $s100, $s1g, $s10g, $ssfp, $ssfpp, $sqsfp ) ); //.. Портов 8-10
				break;
			case 3: return ( byPortsSpeedTest( 16, 20, $Сhoices[1], $s100, $s1g, $s10g, $ssfp, $ssfpp, $sqsfp ) ); //.. Портов 16-20
				break;
			case 4: return ( byPortsSpeedTest( 24, 28, $Сhoices[1], $s100, $s1g, $s10g, $ssfp, $ssfpp, $sqsfp ) ); //.. Портов 24-28
				break;
			case 5: return ( byPortsSpeedTest( 48, 52, $Сhoices[1], $s100, $s1g, $s10g, $ssfp, $ssfpp, $sqsfp ) ); //.. Портов 48-52
				break;
			case 6: return ( byPortsSpeedTest( 52, 100, $Сhoices[1], $s100, $s1g, $s10g, $ssfp, $ssfpp, $sqsfp ) ); //.. Портов >52
				break;
		}
return ( 0 );
}
function byPortsSpeedTest( $begin, $end, $Сhoices, $s100, $s1g, $s10g, $ssfp, $ssfpp, $sqsfp )
{
//.. ТЕСТ::print "\r\n2/$Сhoices/ $s100, $s1g, $s10g, $ssfp, $ssfpp, $sqsfp /$begin, $end,/<br>";
		switch ( $Сhoices ){
			case 0: return (1); //.. НЕВЕРОЯТНО!!!! 
			case 1: if ( $s100 >= $begin && $s100 <= $end ) return (1);
				break;
			case 2: if ( $s1g+$ssfp >= $begin && $s1g+$ssfp <= $end ) return (1);
				break;
			case 3: if ( $s10g+$ssfpp >= $begin && $s10g+$ssfpp <= $end ) return (1);
				break;
			case 4: if ( $sqsfp >= $begin ) return (1);
				break;
		}
return ( 0 );
}
//.......................................................................................
//.. По количеству портов и нужной среды 0 - нет нужных портов: 1 - нашли!
function byPortsSreda ( $Сhoices, $iSumС, $iSumO )
{
//.. ТЕСТ::print "\r\n1/$Сhoices[2]/ $iSumС, $iSumO /<br>";
		switch ( $Сhoices[0] ){
			case 0: return (1); //.. НЕВЕРОЯТНО!!!! 
			case 1:  return ( byPortsSredaTest( 1,5, $Сhoices[2], $iSumС, $iSumO ) ); //.. Портов 5
				break;
			case 2: return ( byPortsSredaTest( 8, 10, $Сhoices[2], $iSumС, $iSumO ) ); //.. Портов 8-10
				break;
			case 3: return ( byPortsSredaTest( 16, 20, $Сhoices[2], $iSumС, $iSumO ) ); //.. Портов 16-20
				break;
			case 4: return ( byPortsSredaTest( 24, 28, $Сhoices[2], $iSumС, $iSumO ) ); //.. Портов 24-28
				break;
			case 5: return ( byPortsSredaTest( 48, 52, $Сhoices[2], $iSumС, $iSumO ) ); //.. Портов 48-52
				break;
			case 6: return ( byPortsSredaTest( 52, 100, $Сhoices[2], $iSumС, $iSumO ) ); //.. Портов >52
				break;
		}
return ( 0 );
}
function byPortsSredaTest ( $begin, $end, $Сhoices, $iSumС, $iSumO )
{
//.. ТЕСТ::print "\r\n2/$Сhoices/ $iSumС, $iSumO /<br>";
		switch ( $Сhoices ){
			case 0: return (1); //.. НЕВЕРОЯТНО!!!! 
			case 1: if ( $iSumС >= $begin && $iSumС <= $end ) return (1);
				break;
			case 2: if ( $iSumO >= $begin && $iSumO <= $end ) return (1);
				break;
		}
return ( 0 );
}
//.......................................................................................
//.......................................................................................
//.. Вывод результатов, аналогично tableOut, но в сокращенном виде
function searchOut( $top, $netData, $choices )
{
//.....................................................
//.. Номер и имя
	$name = 1;
	$part = 2;
	$fullName = 3;
//.. Блок портов
	$c100 = 7;
	$p100 = 8;
	$pp100 = 9;
	$c1k = 10;
	$p1k = 12;
	$pp1k = 13;
	$c10k = 11;
//.. Блок СФП
	$sfp = 14;
	$sfpdp = 15;
	$sfpp = 16;
	$qsfp = 17;
//.. Блок производительность
	$ps = 20;
	$pk = 21;
	$mac = 22;
	$poew = 25;
//..Номер столбца в котором находится имя файла Серии
	$iImageProduct = 28;
	$iImage = 29;
//.....................................................

//.. Вывод заголовка таблицы
$textOut = <<<OUTs
<table style="padding: 10px 0 10px; border-radius: 10px; background-color: #F0F8FF; border: 1px dotted #C4C9D1; color: #0b3d91;">
<tr style="background: #ace7f2;">
	<td>&nbsp;</td>
	<td>$top[$part]</td>
	<td>$top[$fullName]</td>
</tr>
OUTs;
print $textOut;

//.. Тело таблицы
	$iData = count ($netData);
	$Series = $netData[0][0];
	$ImageSeries = $netData[0][$iImage];

	if ( strlen ($ImageSeries) > 1 && file_exists("image/".$ImageSeries) ) 
			$iImages = "<img src=\"image/$ImageSeries\" alt=\"$Series\" border=\"0\" height=\"100\" style=\"vertical-align: middle;\" />";
		else $iImages = "";
	
$textOut = <<<OUTs
<tr height="50">
	<td colspan="3" style="text-align: center; border-top: 1px solid; border-bottom: 1px dotted;">$iImages<b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$Series</b></td>
</tr>
OUTs;

print $textOut;	
	
	for ( $j=0, $k=0; $j < $iData; $j++){
		//.. Проверяем изменилась ли "серия" коммутаторов или нет
		if ( strcmp ($Series, $netData[$j][0]) ){
			
			$Series = $netData[$j][0];
			$ImageSeries = $netData[$j][$iImage];
			
			$k=0; //... Это для переключения цвета полоски!
			
			//.. Проверяем есть ли изображение к данной серии
			if ( strlen ($ImageSeries) > 1 && file_exists("image/".$ImageSeries) ) { 
					//.. Если изображение есть, то мы перед именем серии выводим изображение
					//.. 22/09 - Нет, так не пойдет. Для вывода картинки нужно чтобы она еще и на диске была. Это надо проверить!!!!

$textOut = <<<OUTs
<tr height="50">
	<td colspan="3" align="center" style="border-top: 1px solid #C4C9D1; border-bottom: 1px dotted;"><img src="image/$ImageSeries" alt="$Series" border="0" height="100" style="vertical-align: middle;" /><b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$Series</b></td>
</tr>
OUTs;
			
				} else { //.. Иначе выводим только название Серии
			
$textOut = <<<OUTs
<tr height="50">
	<td colspan="3" align="center" style="border-top: 1px solid #C4C9D1; border-bottom: 1px dotted;"><b>$Series</b></td>
</tr>
OUTs;
			}

print $textOut;		
		}
	
	

		//.. Определяем цвет выводимой полоски ( по переменной $k)
		if ( $k%2 ) $tr = "<tr align=\"center\">"; 
			else 
			$tr = "<tr align=\"center\" style=\"color: #0b3d91; background: #f0ffff;\">";
		$k++;
		
//.. Блок вывода строки данных по модели
		if ( strlen ($netData[$j][$iImageProduct]) > 1 && file_exists("image/".$netData[$j][$iImageProduct]) ){
				$ImageProduct = "<img src=\"image/".$netData[$j][$iImageProduct]."\" border=0 height=100 style=\"vertical-align: middle\" />";
			} else $ImageProduct = "&nbsp;";
		
		$i1 = $netData[$j][$part];
		$i3 = str_replace("\\\\","<br>",$netData[$j][$fullName]); //$netData[$j][$fullName];
		
$textOut = <<<OUTs
$tr
	<td>$ImageProduct</td>
	<td>$i1</td>
	<td align="left">$i3</td>
</tr>
OUTs;
print $textOut;
	
	}
	
//.. Финал таблицы
print "<table>";
}
//.......................................................................................
//.......................................................................................
//.. Работа с переменными, которые мы передаем между программами...
function setSession ( $Description, $Products )
{
	if ( !isset ($_SESSION['mProd']) ) {
		$_SESSION['mProd'] = $Products;
		$_SESSION['mDesc'] = $Description;
	} 
}

function getSession ( )
{
	if ( isset ($_SESSION['mProd']) ) {
			$massOut = $_SESSION['mProd'];
			$Top = $_SESSION['mDesc'];
		} else {
			$massOut[0] = 0;
			$Top[0] = 0;
	}
return array ( $Top, $massOut );
}
//...........................................................................................................................
?>