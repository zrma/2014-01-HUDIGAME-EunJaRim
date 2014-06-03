<!DOCTYPE html>
<html lang="en" class="theme-winter">
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <meta name="title" content="YaMangOnline!" />
        <title>YaMangOnline!</title>

                <!--[if IE]><![endif]-->

        <meta http-equiv="X-UA-Compatible" content="IE=edge" />

        <script type="text/javascript" language="javascript">
		
		<?php
			// not include in github repository!
			
			
			$facebookID = $_POST["id"];
			$facebookToken = $_POST["token"];
			//$facebookName = $_POST["name"];
			$facebookName = "";
			
			
			if($facebookToken!=NULL) {
				echo "var m_AccessToken = '".$facebookToken."';";
			} else {
				echo 'var m_AccessToken = "";';
			}
			if($facebookID!=NULL) {
				echo 'var m_FaceBookID = '.$facebookID.';';
				login($facebookID);
			} else {
				echo 'var FaceBookID = "";';
			}
			/*
			if($_POST["name"]!=NULL) {
				echo 'var m_PlayerName = '.$_POST["name"].';';
			} else {
				echo 'var m_PlayerName = "";';
			}
			*/
			
			function login($facebookID) {
				
				// not include in github repository
				include 'db_config.php';
				
				$checkLogin = "SELECT game_id FROM YAMANG_USER WHERE facebook_id =$facebookID";
				$resultCheckLogin = mysqli_query($connect, $checkLogin);
				
				
				// 누구 프로시져좀 짜주시죠 (웃음)
				$row = mysqli_fetch_array($resultCheckLogin);
				{
					
					if ( $row['game_id'] == ""  || $row['game_id'] == NULL ) {
						// 가입이 안되어 있으면 먼저 유저테이블에 데이터를 넣음
						
						// idx 가져오기
						$idxQuery = "SELECT idx FROM YAMANG_USER ORDER BY `idx` DESC LIMIT 1";
						$idxResult = mysqli_query($connect, $idxQuery);
						
						$index = -1;
						while($rowIdx = mysqli_fetch_array($idxResult)) {
							if ( $rowIdx['idx'] != ""  || $rowIdx['idx'] != NULL ) {
								$index = $rowIdx['idx'];
								$index = $index + 1000;
							}
						}
						
						if ( $index == -1 ) {
							echo "index == -1";
							//return;
						}
						
						$signUpQuery = "INSERT INTO `YAMANG_USER` (`facebook_id`,`game_id`,`win`,`lose`,`error`,`name`) VALUES ('$facebookID', '$index', 0, 0, 0, '$facebookName')";
						mysqli_query($connect, $signUpQuery);
						
					}
					
					$gameID = -1;
					$checkLogin2 = "SELECT game_id FROM YAMANG_USER WHERE facebook_id =".$facebookID;
					$resultCheckLogin2 = mysqli_query($connect, $checkLogin2);
					while($row2 = mysqli_fetch_array($resultCheckLogin2)) {
						$gameID = $row['game_id'];
					}
					
					echo 'var m_PlayerID = '.$gameID.';';
				}
				
				mysqli_close($connect);
			}
		?>
		
			function getPlayerID() {
				// test
				var min = 1000;
				var max = 1100;
				return Math.floor(Math.random() * (max - min + 1) + min);
			}
			
			function RoomEnter(room) {
				if(m_PlayerID == "") {
					window.location.href = "loaderTemplate.html?r=" + room + "&i=" + getPlayerID();
					window.location.href=window.location.href;		
				} else {
					window.location.href = "loaderTemplate.html?r=" + room + "&i=" + m_PlayerID;
					window.location.href=window.location.href;			
				}
			}
			
			function LoginFaceBook() {
				window.location.href = "loaderTemplate.html?status=loginProcess";
				window.location.href=window.location.href;	
			}
			
			
		</script>
				
			
        <link rel="shortcut icon" href="/favicon.ico" />
        <link rel="apple-touch-icon" href="/apple-touch-icon.png" />
        <link rel="meta" href="/labels.rdf" type="application/rdf+xml" title="ICRA labels" />
		
    <script type="text/javascript" src="./loaderAsset/js/all.js"></script>
<link rel="stylesheet" type="text/css" media="screen" href="./loaderAsset/css/all.css" />
<link rel="stylesheet" type="text/css" media="screen" href="./loaderAsset/css/credit.css" />
<script type="text/javascript">var _gaq = _gaq || [];_gaq.push(['_setAccount', 'UA-22355600-1']);_gaq.push(['_trackPageview']);</script><!--[if  IE 6]><link rel="stylesheet" type="text/css" media="screen" href="http://cdn.battlefield.play4free.com/en/static/20140225100054/all/css/ie6" />
<![endif]--></head>
        <!--[if lt IE 7 ]> <body class="ie6 index lang-en credits web" id="credits"> <![endif]-->
    <!--[if IE 7 ]>    <body class="ie7 index lang-en credits web" id="credits"> <![endif]-->
    <!--[if IE 8 ]>    <body class="ie8 index lang-en credits web" id="credits"> <![endif]-->
    <!--[if IE 9 ]>    <body class="ie9 index lang-en credits web" id="credits"> <![endif]-->
    <!--[if !(IE)]>
    <!--> <body class="index lang-en credits web" id="credits">
	
	

			<script>
                (function () {
                    var tmp = document.documentMode, e, isIE, isIE8plus, ieVersion = 0;
                    try{document.documentMode = "";}
                    catch(e){ };
                    isIE8plus = typeof document.documentMode == "number";
                    if (/*@cc_on!@*/false) { ieVersion = 10; }
                    else if (isIE8plus) { ieVersion = 11; }
                    console.log(ieVersion);
                    try{document.documentMode = tmp;}
                    catch(e){ };
                    if (ieVersion > 0) {
                        document.body.className += ' ie' + ieVersion;
                    }
                })();
            </script><!--<![endif]-->
	
<div id="p4fnav-topnav-full" class="p4fnav-topnav-full origindark" style="opacity: 0; filter: alpha(opacity = 0);">
	<div class="p4fnav-menu-block">
		
		<a id="p4fnav-p4f-logo-full" href="http://www.ea.com" class="ea-logo en"></a>
		
		<div class="p4fnav-block">
			<!-- More Games Label -->
			<div class="p4fnav-more-games-wrapper" style="">
				<a id="p4fnav-more-games-btn" class="p4fnav-more-games-btn en" href="#" style="">
					<div>More <b>Free Games</b></div>
				</a>
			</div>

			<!-- Language Selector Menu -->
			
				<div class="p4fnav-lang-menu-wrapper">
					<div id="p4fnav-lang-selector-dropdown" class="p4fnav-lang-menu" style="display:none;">
						<div class="p4fnav-lang-lbl"><b>Language</b></div>
						<input id="p4fnav-supported-lang-regex" type="hidden" value="(en|de|fr|pl|ru)" />
						
							<div class="p4fnav-lang-separator"></div>
							<a href="/en/${uri}" class="p4fnav-lang-lbl">English</a>
						
							
							<a href="/de/${uri}" class="p4fnav-lang-lbl">Deutsch</a>
						
							<div class="p4fnav-lang-separator"></div>
							<a href="/fr/${uri}" class="p4fnav-lang-lbl">Français</a>
						
							
							<a href="/pl/${uri}" class="p4fnav-lang-lbl">Polski</a>
						
							<div class="p4fnav-lang-separator"></div>
							<a href="/ru/${uri}" class="p4fnav-lang-lbl">Русский</a>
						
					</div>

					<!-- Language Selector -->
					<div id="p4fnav-lang-selector-btn" class="p4fnav-lang-selector">
						<div class="p4fnav-lang-globe"></div>
						<a href="#" class="p4fnav-lang-lbl">English</a>
						<div class="p4fnav-lang-ddarrow"></div>
					</div>
				</div>
			

			

			

			
				
				
				
				
			
			<div id="p4fnav-login-block" style="display:block;float:right;">
				
				
			</div>
			<div id="p4fnav-logout-block" style="display:none;float:right;">
				
			</div>

			<div class="p4fnav-clear"></div>

			<!-- Menu -->
			<div id="p4fnav-more-games-dropdown" class="p4fnav-menu" style="display:none;">
			
				<a id="p4fnav-gamelink-2" href="http://www.battlefieldheroes.com/en/?utm_source=battlefield.play4free.com&utm_medium=referral&utm_campaign=p4f-common-nav">
					<div class="p4fnav-game-item">
						<div class="p4fnav-game">
							<div class="p4fnav-game-logo en bfh"></div>
						</div>
						<div class="p4fnav-game-details">
							<div class="p4fnav-game-title">BATTLEFIELD Heroes</div>
							<div class="p4fnav-clear"></div>
							<div class="p4fnav-game-genres">Shooter</div>
							<div class="p4fnav-clear"></div>
							<div class="p4fnav-game-download">Download</div>
						</div>
					</div>
				</a>

				<div class="p4fnav-clear"></div>
				<div class="p4fnav-separator"></div>

				<a id="p4fnav-gamelink-1" href="http://battlefield.play4free.com/en/?utm_source=battlefield.play4free.com&utm_medium=referral&utm_campaign=p4f-common-nav">
					<div class="p4fnav-game-item">
						<div class="p4fnav-game">
							<div class="p4fnav-game-logo en bfp4f"></div>
						</div>
						<div class="p4fnav-game-details">
							<div class="p4fnav-game-title">BATTLEFIELD: Play4Free</div>
							<div class="p4fnav-clear"></div>
							<div class="p4fnav-game-genres">Shooter</div>
							<div class="p4fnav-clear"></div>
							<div class="p4fnav-game-download">Download</div>
						</div>
					</div>
				</a>

				<div class="p4fnav-clear"></div>
				<div class="p4fnav-separator"></div>

				<a id="p4fnav-gamelink-6" href="http://www.tiberiumalliances.com/en/?utm_source=battlefield.play4free.com&utm_medium=referral&utm_campaign=p4f-common-nav">
					<div class="p4fnav-game-item">
						<div class="p4fnav-game">
							<div class="p4fnav-game-logo en cca"></div>
						</div>
						<div class="p4fnav-game-details">
							<div class="p4fnav-game-title">Command & Conquer:  Tiberium Alliances</div>
							<div class="p4fnav-clear"></div>
							<div class="p4fnav-game-genres">Strategy</div>
							<div class="p4fnav-clear"></div>
							<div class="p4fnav-game-download">Browser</div>
						</div>
					</div>
				</a>

				<div class="p4fnav-clear"></div>
				<div class="p4fnav-separator"></div>

				<a id="p4fnav-gamelink-4" href="http://www.easportsfifaworld.com/en/?utm_source=battlefield.play4free.com&utm_medium=referral&utm_campaign=p4f-common-nav">
					<div class="p4fnav-game-item">
						<div class="p4fnav-game">
							<div class="p4fnav-game-logo en fifaw"></div>
						</div>
						<div class="p4fnav-game-details">
							<div class="p4fnav-game-title">FIFA World</div>
							<div class="p4fnav-clear"></div>
							<div class="p4fnav-game-genres">Sports</div>
							<div class="p4fnav-clear"></div>
							<div class="p4fnav-game-download">Download</div>
						</div>
					</div>
				</a>

				<div class="p4fnav-clear"></div>
				<div class="p4fnav-separator"></div>

				<a id="p4fnav-gamelink-3" href="http://world.needforspeed.com/?lang=en&utm_source=battlefield.play4free.com&utm_medium=referral&utm_campaign=p4f-common-nav">
					<div class="p4fnav-game-item">
						<div class="p4fnav-game">
							<div class="p4fnav-game-logo en nfsw"></div>
						</div>
						<div class="p4fnav-game-details">
							<div class="p4fnav-game-title">Need for Speed World</div>
							<div class="p4fnav-clear"></div>
							<div class="p4fnav-game-genres">Racing</div>
							<div class="p4fnav-clear"></div>
							<div class="p4fnav-game-download">Download</div>
						</div>
					</div>
				</a>

				
					<div class="p4fnav-clear"></div>
					<div class="p4fnav-separator"></div>


					<div class="p4fnav-game-item">
						<a id="p4fnav-gamelink-8" class="p4fnav-game-logo origin" href="http://free.origin.com/?utm_source=battlefield.play4free.com&utm_medium=referral&utm_campaign=p4f-common-nav" target="_blank"></a>
					</div>


					<div class="p4fnav-clear"></div>
					<div class="p4fnav-separator"></div>

					<div class="p4fnav-view-all">
						<a id="p4fnav-gamelink-9" class="" href="http://free.origin.com/?utm_source=battlefield.play4free.com&utm_medium=referral&utm_campaign=p4f-common-nav" target="_blank">View All <span class="arrow">&#x25B6;</span></a>
					</div>
				

				
			
			</div>
		</div>
	</div>
</div>
        <div id="wrapper">
            <div id="header" class="header clearfix">
                <div class="logo"><a href="/en/"><img src="./loaderAsset/images/logo.png" alt="Battlefield Play4Free"/></a></div>
<div class="user-profile notloggedin closed">
	<div class="login-popup">
		<form class="magmaForm login-form" method="post" action="https://battlefield.play4free.com/en/user/login?destination=%2Fen%2Fcredits">
		

		<button class="button-view button-view-primary button-view-login" value="" tabindex="4"><i class="glyphs glyphs-arrow"></i><span>Log in</span></button>    <div class="login-extras">
    </div>
</form>
	</div>
</div>
<a href="/en/user/signup" class="signup-button">Sign up</a>

﻿<div class="social-links-wrap">	
	<ul class="social-links">		
		<li class="facebook"><a target="_blank" href="http://facebook.com/bfplay4free">Facebook</a></li>
		<li class="twitter"><a target="_blank" href="http://twitter.com/bfplay4free">Twitter</a></li>
		<li class="youtube"><a target="_blank" href="http://youtube.com/bfplay4free">Youtube</a></li>
	</ul>
</div>                <div id="main_menu" class="menu">
                      <ul>
                        <li class=" menu-item1">
      <a class="menu-label" href="/en/">Home</a>
    </li>      
        
                          
    
    <li class="with-submenu selected menu-item2">
    <span class="menu-label">About</span>
      <span class="submenu">
        <ul class="submenu-list">
                    <li ><a href="/en/help">Help</a></li>
                    <li ><a href="/en/hardware">Hardware requirements</a></li>
                    <li ><a href="/en/media">Media</a></li>
                    <li class="selected"><a href="/en/credits">Credits</a></li>
                  </ul>
        <span class="submenu-bg"></span>
      </span>
    </li>  
        
                          
    
    <li class="with-submenu  menu-item3">
    <span class="menu-label">Community</span>
      <span class="submenu">
        <ul class="submenu-list">
                    <li ><a href="/en/forum/forum">Forum</a></li>
                  </ul>
        <span class="submenu-bg"></span>
      </span>
    </li>  
        
                          
    
    <li class="with-submenu  menu-item4">
    <span class="menu-label">Store</span>
      <span class="submenu">
        <ul class="submenu-list">
                    <li ><a href="/en/store/purchaseFunds">Get Play4Free Funds</a></li>
                    <li ><a href="/en/rsp">Rent a server</a></li>
                    <li ><a href="/en/extras/buyCharacterSlots">Get more slots</a></li>
                    <li ><a href="/en/store/redeem">Redeem a code</a></li>
                    <li ><a href="/en/store/qa">Play4Free Funds Q&A</a></li>
                  </ul>
        <span class="submenu-bg"></span>
      </span>
    </li>  
        
       
      </ul>                </div>
            </div>
            <div id="page" class="clearfix">
                                <div id="content" class="clearfix"><div id="credits-wrapper" class="box-wrapper">
	<div id="credits-inner" class="box-fullwidth">
		<div class="credits">
			<div class="credits-content">
				<h1 class="credits-header">현재 참여 가능 전장</h1>
				<span class="credits-order">참여 가능 방수 : 6</span>
				<div class="credits-left">
					<a href="javascript:RoomEnter(1);"><h2>Room.1</h2></a>
					<ul>
						<li>sm9<span>정1품</span></li>
						<li>대기중<span>-----</span></li>
						<li>&nbsp;</li>
						<li>&nbsp;</li>
					</ul>
					<a href="javascript:RoomEnter(2);"><h2>Room.2</h2></a>
					<ul>
						<li>agebreak<span>정1품</span></li>
						<li>대기중<span>-----</span></li>
						<li>&nbsp;</li>
						<li>&nbsp;</li>
					</ul>
				</div>
				<div class="credits-left">
					<a href="javascript:RoomEnter(3);"><h2>Room.3</h2></a>
					<ul>
						<li>대기중<span>-----</span></li>
						<li>대기중<span>-----</span></li>
						<li>&nbsp;</li>
						<li>&nbsp;</li>
					</ul>
					<a href="javascript:RoomEnter(4);"><h2>Room.4</h2></a>
					<ul>
						<li>대기중<span>-----</span></li>
						<li>대기중<span>-----</span></li>
						<li>&nbsp;</li>
						<li>&nbsp;</li>
					</ul>
				</div>
				<div class="credits-left">
					<a href="javascript:RoomEnter(5);"><h2>Room.5</h2></a>
					<ul>
						<li>대기중<span>-----</span></li>
						<li>대기중<span>-----</span></li>
						<li>&nbsp;</li>
						<li>&nbsp;</li>
					</ul>
					<a href="javascript:RoomEnter(6);"><h2>Room.6</h2></a>
					<ul>
						<li>대기중<span>-----</span></li>
						<li>대기중<span>-----</span></li>
						<li>&nbsp;</li>
						<li>&nbsp;</li>
					</ul>
				</div>
			</div>
			<div class="credits-content additional">
			<h1 class="credits-header">현재 전쟁중인 전장</h1>
				<div class="credits-col">
					<a href="javascript:LoginFaceBook();"><h2>Room.10(LoginTest)</h2></a>
					<ul>
						<li>자바지기</li>
						<li>요츠바짱</li>
					</ul>
					<h2>Room.11</h2>
					<ul>
						<li>Trizdreaming</li>
						<li>Ponyo</li>
					</ul>
					<h2>Room.12</h2>
					<ul>
						<li>ajkee</li>
						<li>EashComplete</li>
					</ul>
				</div>
				<div class="credits-col">
					<h2>Room.13</h2>
					<ul>
						<li>godrmnext</li>
						<li>nigashow</li>
					</ul>
					<h2>Room.14</h2>
					<ul>
						<li>zeliard</li>
						<li>zrma</li>
					</ul>
					<h2>Room.15</h2>
					<ul>
						<li>Poppy</li>
						<li>Sehun</li>
					</ul>
				</div>
				<div class="credits-col">
					<h2>Room.16</h2>
					<ul>
						<li>Anonymous01</li>
						<li>Anonymous02</li>
					</ul>
					<h2>Room.17</h2>
					<ul>
						<li>Anonymous03</li>
						<li>Anonymous04</li>
					</ul>
					<h2>Room.18</h2>
					<ul>
						<li>Anonymous05</li>
						<li>Anonymous06</li>
					</ul>
				</div>
				<div class="credits-col">
					<h2>Room.19</h2>
					<ul>
						<li>Anonymous07</li>
						<li>Anonymous08</li>
					</ul>
					<h2>Room.20</h2>
					<ul>
						<li>Anonymous09</li>
						<li>Anonymous10</li>
					</ul>
					<h2>Room.21</h2>
					<ul>
						<li>Anonymous11</li>
						<li>Anonymous12</li>
					</ul>
				</div>
				<div class="credits-col">
					<h2>Room.22</h2>
					<ul>
						<li>Anonymous13</li>
						<li>Anonymous14</li>
					</ul>
					<h2>Room.23</h2>
					<ul>
						<li>Anonymous15</li>
						<li>Anonymous16</li>
					</ul>
					<h2>Room.24</h2>
					<ul>
						<li>Anonymous17</li>
						<li>Anonymous18</li>
					</ul>
				</div>
				<div class="credits-col">
					<h2>Room.25</h2>
					<ul>
						<li>Anonymous19</li>
						<li>Anonymous20</li>
					</ul>
					<h2>Room.26</h2>
					<ul>
						<li>Anonymous21</li>
						<li>Anonymous22</li>
					</ul>
					<h2>Room.27</h2>
					<ul>
						<li>Anonymous23</li>
						<li>Anonymous24</li>
					</ul>
				</div>
				<div class="credits-bottom">
					<p>이 데이터는 과거의 데이터로 실제 전장 상황과 다를 수 있습니다.</p>
					<div class="fb-login-button" data-max-rows="1" data-size="xlarge" data-show-faces="false" data-auto-logout-link="false"></div>

											<fb:login-button scope="public_profile,email" onlogin="checkLoginState();">
</fb:login-button>
<div id="status">
				</div>
			</div>
		</div>
	</div>
</div></div>
            </div>
            <div id="footer" class="clearfix">
                ﻿<div id="footer-wrap">
	<ul id="links">
		<!--<li id="awlink"><a target="_blank" href="http://lt.dell.com/lt/lt.aspx?CID=64503&LID=1616494&DGC=BA&DGSeg=DHS&DURL=http://www.alienware.com"><span>Plays best on Alienware</span></a></li>-->
		<li id="ealink"><a target="_blank" href="http://ea.com/"><span>EA</span></a></li>
		<li id="p4flink"><a target="_blank" href="http://play4free.com"><span>Play4Free</span></a></li>
		<li id="easylink"><a target="_blank" href="http://easystudios.se"><span>Easy Studios</span></a></li>
              <li id="pegilink"><a target="_blank" href="http://pegi.info/"><span>PEGI</span></a></li>
      		<li id="corrodedlink"><a target="_blank" href="http://www.ninetone.com/records/corroded">Theme music by<span>Corroded</span></a></li>
	</ul>

	<p id="notice">
		<a id="legalnotice" href="http://www.nhnnext.org">&copy; 2014 Eunjarim Inc. All rights reserved.</a>
		<br />
		<a id="aboutus" target="_blank" href="http://easystudios.se/">About us</a> |
		<a id="onlinesafteyguidelines" href="http://www.ea.com/uk/1/online-safety">Online Safety Guidelines</a> |
		<a id="pp" href="http://www.ea.com/1/privacy-policy">Privacy policy</a> |
		<a id="eula" href="/user/euala/en?p=true">EUALA</a> |
		<a id="tos" href="http://tos.ea.com/legalapp/WEBTERMS/en/en/PC/">Terms of service</a> |
		<a id="roc" href="/user/roc/en">Rules of conduct</a>
		<!--
		<a id="codeofconduct" href="/user/roc/en">Rules of conduct</a> | 
		<a id="jobs" href="http://easystudios.se/">WEB_FOOTER_JOBS</a>-->
	</p>
    <div class="language">
	<h3>Switch language: </h3>
	<ul>
			<li class="lang-en">
					<span lang="en">English</span>
		         | 		</li>
			<li class="lang-de">
					<a lang="de" href="/de/credits">Deutsch</a>
		         | 		</li>
			<li class="lang-ru">
					<a lang="ru" href="/ru/credits">Русский</a>
		         | 		</li>
			<li class="lang-fr">
					<a lang="fr" href="/fr/credits">Français</a>
		         | 		</li>
			<li class="lang-pl">
					<a lang="pl" href="/pl/credits">Polski</a>
		        		</li>
		</ul>
</div>
</div>            </div>
        </div>
        <div id="js-msgs">
  <div id="add-to-friends">WEB_PLAYER_PROFILE_ADD_TO_FRIENDS</div>
  <div id="remove-from-friends">WEB_PLAYER_PROFILE_REMOVE_FROM_FRIENDS</div>
  <div id="friendlist-url">/en/ajax/checkFriendStatus</div>
  <div id="friend-url">/en/ajax/friend</div>
</div>
        
        
        <div id="magma-cssready"></div>
        <div class="game-playnow-msg">
            <div class="playnow-windows-xp-requirement-message">Battlefield Play4Free requires Windows XP or newer, sorry!</div>
            <div class="playnow-internet-explorer-upgrade-message">Please upgrade to Internet Explorer 5 or newer.</div> 
            <div class="playnow-internet-explorer-64bit-message">The 64bit version of Internet Explorer is not currently supported, please use the 32bit version.</div> 
            <div class="playnow-firefox-upgrade-message">Please upgrade to Firefox 1.5 or newer.</div>
            <div class="playnow-try-compatible-browser-message">Please try Internet Explorer, Firefox or Chrome.</div>
            <div class="playnow-not-compatible-browser-message">Battlefield Play4Free does not currently work with your browser.</div>
        </div>        
        
        <div class="playnowmsg-wrapper"></div>

		<script>
(function (root) {
	"use strict";
	
	var APP = root.APP,
		nsUser = root.APP.ns('user');


	APP.session.unset('user');
	APP.session.unset('soldiers');
	APP.session.unset('wallet');
	
}(this))
</script>
</body>
</html>
