<?php
include "./conf_telegram_bot.php";

function telegram_bot_send($message, $log_level) // multiple defs
{
    global $conf;
    $text = trim($message);
    $chat_id = $conf['trusted']['DEBUG'];
    
    if($log_level=='test')
    {
        $chat_id = $conf['trusted']['TEST'];
    }

    if (strlen(trim($text)) > 0) 
    {
        $send = "https://api.telegram.org/bot".$conf['bot_token']."/sendmessage?parse_mode=html&chat_id=".$chat_id ."&text=" . urlencode($text);
        file_get_contents($send);  
    }

}
//echo "telegrame bot included.";
?>