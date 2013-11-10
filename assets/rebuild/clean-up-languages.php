#!/usr/bin/env drush

<?php

/**
 * @file
 * Does the same work as clean_up_languages.sh
 */

$alias = drush_sitealias_get_record('@warmshowers.dev');
if (!$alias) {
  return drush_set_error('NO_ALIAS_FOUND', dt('Failed to load alias.'));
}

$query = "DROP TABLE IF EXISTS `languages`;";
drush_invoke_process('@warmshowers.dev', 'sql-query', array($query));
$query = "CREATE TABLE `languages` (
  `language` varchar(12) NOT NULL DEFAULT '',
  `name` varchar(64) NOT NULL DEFAULT '',
  `native` varchar(64) NOT NULL DEFAULT '',
  `direction` int(11) NOT NULL DEFAULT '0',
  `enabled` int(11) NOT NULL DEFAULT '0',
  `plurals` int(11) NOT NULL DEFAULT '0',
  `formula` varchar(128) NOT NULL DEFAULT '',
  `domain` varchar(128) NOT NULL DEFAULT '',
  `prefix` varchar(128) NOT NULL DEFAULT '',
  `weight` int(11) NOT NULL DEFAULT '0',
  `javascript` varchar(32) NOT NULL DEFAULT '',
  PRIMARY KEY (`language`),
  KEY `list` (`weight`,`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;";

drush_invoke_process('@warmshowers.dev', 'sql-query', array($query));

$query = 'INSERT INTO `languages` VALUES (\'cs\',\'Czech\',\'Čeština\',0,0,3,\'(((($n%10)==1)&&(($n%100)!=11))?(0):((((($n%10)>=2)&&(($n%10)<=4))&&((($n%100)<10)||(($n%100)>=20)))?(1):2))\',\'http://cs.warmshowers.dev\',\'\',7,\'\'),(\'de\',\'German\',\'Deutsch\',0,1,2,\'($n!=1)\',\'http://de.warmshowers.dev\',\'\',3,\'af0ad6109adef9b7a6290f2dca86561f\'),(\'en\',\'English\',\'English\',0,0,0,\'\',\'\',\'en\',0,\'\'),(\'en-working\',\'English\',\'English\',0,1,2,\'($n!=1)\',\'http://warmshowers.dev\',\'\',0,\'\'),(\'es\',\'Spanish\',\'Español\',0,1,2,\'($n!=1)\',\'http://es.warmshowers.dev\',\'\',2,\'ada4d3d7443ce6139831fc94f3382382\'),(\'fa\',\'Persian\',\'فارسی\',1,1,2,\'($n!=1)\',\'http://fa.warmshowers.dev\',\'\',9,\'c5f5c97d14b4cbd73db330b433b466de\'),(\'fr\',\'French\',\'Français\',0,1,2,\'($n>1)\',\'http://fr.warmshowers.dev\',\'\',1,\'123219c9e0b22d13414afbfb67057f37\'),(\'it\',\'Italian\',\'Italiano\',0,1,2,\'($n!=1)\',\'http://it.warmshowers.dev\',\'\',4,\'237aa0601be8343d62610d131bce70db\'),(\'ja\',\'Japanese\',\'日本語\',0,1,2,\'($n!=1)\',\'http://ja.warmshowers.dev\',\'\',9,\'f843333e8f6710eb145bdbc9d1efba1c\'),(\'pl\',\'Polish\',\'Polski\',0,1,3,\'(($n==1)?(0):((((($n%10)>=2)&&(($n%10)<=4))&&((($n%100)<10)||(($n%100)>=20)))?(1):2))\',\'http://pl.warmshowers.dev\',\'\',7,\'c09075b7e75f4eb8c33db980ab5c3f7b\'),(\'pt-br\',\'Portuguese\',\'Português\',0,1,2,\'($n!=1)\',\'http://pt.warmshowers.dev\',\'\',5,\'fa84ca25f456723e459cf67b534541b0\'),(\'ro\',\'Romanian\',\'Română\',0,1,3,\'(($n==1)?(0):((($n==0)||((($n%100)>0)&&(($n%100)<20)))?(1):2))\',\'http://ro.warmshowers.dev\',\'\',9,\'378df767317dd887672f8b30f9f83b44\'),(\'ru\',\'Russian\',\'Русский\',0,1,3,\'(((($n%10)==1)&&(($n%100)!=11))?(0):((((($n%10)>=2)&&(($n%10)<=4))&&((($n%100)<10)||(($n%100)>=20)))?(1):2))\',\'http://ru.warmshowers.dev\',\'\',9,\'0a1111b33460f18f2eca440d62e3030b\'),(\'sr\',\'Serbian\',\'Српски\',0,1,3,\'(((($n%10)==1)&&(($n%100)!=11))?(0):((((($n%10)>=2)&&(($n%10)<=4))&&((($n%100)<10)||(($n%100)>=20)))?(1):2))\',\'http://rs.warmshowers.dev\',\'\',9,\'2eaaf7a474674228abd0bc8f3d16865d\'),(\'tr\',\'Turkish\',\'Türkçe\',0,1,0,\'\',\'http://tr.warmshowers.dev\',\'\',10,\'bf5ba802fb556cfa3a462f65bbc695e8\'),(\'zh-hans\',\'Chinese, Simplified\',\'简体中文\',0,1,2,\'($n!=1)\',\'http://cn.warmshowers.dev\',\'\',8,\'3c4534e7fb043a8e2601b34303acbeb5\');';
drush_invoke_process('@warmshowers.dev', 'sql-query', array($query));

drush_invoke_process('@warmshowers.dev', 'cache-clear', array('all'));

// This is the "Clean up Variable" code from i18n.
$variables = i18n_variable();
if ($variables) {
  db_query("DELETE FROM {i18n_variable} WHERE name NOT IN (" . db_placeholders($variables, 'varchar') . ')', $variables);
}
