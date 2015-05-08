<?php
/**
 * @file
 * ws_d7_upgrade_features.variable.inc
 */

/**
 * Implements hook_variable_realm_default_variables().
 */
function ws_d7_upgrade_features_variable_realm_default_variables() {
$realm_variables = array();
  $realm_variables['language']['cs'] = array(
  'comment_notify_default_mailtext' => 'Uživateli [comment-subscribed:author:fullname],

Příspěvek [comment:author:fullname] je komentován: "[comment:node:title]":

----
[comment:title]
[comment:body]
----
Komentář si můžete prohlédnout a reagovat na něj <a href="[comment:url]">kliknutím sem</a>. (Prosíme neodpovídejte pomocí e-mailu, protože ten obdrží pouze správci webu.)

Zrušit odesílání e-mailů, upozorňujících na komentáře k tomuto příspěvku můžete <a href="[comment-subscribed:unsubscribe-url]">kliknutím sem</a>.

- Tým Warmshowers.org',
  'contact_form_information' => '<p>Můžete nás kontaktovat prostřednictvím e-mailu.</p>

<p>Jestliže potřebujete pomoci s používání tohoto webu, přečtěte si nejprve <a href="/faq">FAQ – často kladené dotazy</a>, a pak zvažte, zda si nepřečíst i <a href="http://www.warmshowers.org/forums/website-help-and-support">Fórum</a> a nepožádat o pomoc zde; odpovědi pak mohou pomoci i jiným uživatelům.</p>

<p>If you speak English it will help the admins a *lot* if you can ask your question in English. Thanks! </p>',
  'node_notify_default_mailtext' => 'Milý uživateli jménem [comment:node:author:fullname],

Obdrželi jsme komentář na: "[comment:node:title]":

----
[comment:title]
[comment:body]
----
Komentář si můžete prohlédnout <a href="[comment:url]">kliknutím sem</a>.  (Neodpovídejte prosím e-mailem, protože ten obrží pouze správci webu.)

Při každé reakci na váš příspěvek bude upozorňován e-mailems.

- Tým Warmshowers',
  'user_mail_password_reset_body' => 'Milý uživateli jménem [user:name],

vy (nebo někdo jiný) jste požadoval jednorázové přihlášení k vašemu účtu na Warmshowers.org. Pokud jste to nebyl vy, tuto zprávu ignorujte.

Klikněte prosím <a href="[user:one-time-login-url]">sem pro přihlášení a vymazání vašeho hesla</a>. Nemůžete-li na tento odkaz kliknout, zkopírujte ho a vložte do adresního řádku vašeho prohlížeče:

[user:one-time-login-url]

Tento odkaz slouží pro jednorázové přihlášení, odkaz tedy lze použít pouze jednou. Jeho platnost vyprší za 24 hodin a pokud ho nepoužijete, nic se nestane.

Po přihlášení budete přesměrován na <a href="[user:edit-url]">stránku pro úpravy vašeho profilu</a>, kde můžete změnit své heslo, resp. nastavit nové.

Díky,
-Randy Fay a registrační tým
WarmShowers.org

Co dělat při potížích:

Když obdržíte zprávu "Bohužel nemáte přístup na tuto stránku (error 403)", většinou to znamená, že máte ve svém prohlížeči zakázané cookies. Jak správně nastavit prohlížeč je popsáno v <a href="http://www.warmshowers.org/content/i-try-log-it-still-says-im-not-logged">tomto článku v často kladených dotazech (FAQ)</a>.

Nemůžete-li tento problém vyřešit, požádejte nás o pomoc; odpovězte na tento e-mail a problém popište.
',
  'user_mail_password_reset_subject' => '[Warmshowers.org] Odkaz na jednorázové přihlášení pro uživatele [user:name]',
  'user_mail_register_admin_created_body' => '[user:name],

A site administrator at !site has created an account for you. You may now log in to !login_uri using the following username and password:

username: [user:name]


You may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you will be redirected to [user:edit-url] so you can change your password.


--  !site team',
  'user_mail_register_admin_created_subject' => 'An administrator created an account for you at  !site',
  'user_mail_register_no_approval_required_body' => '[user:name],

Děkujeme, že jste se registroval/registrovala na Warmshowers.org.

Kliknutím <a href="[user:one-time-login-url]">sem</a> ověříte správnost vaší e-mailové adresy a aktivujete tím svůj účet. (Nemůžete-li na odkaz kliknout, zkopírujte ho a vložte do adresního řádku vašeho prohlížeče:

[user:one-time-login-url]

Po ověření výše uvedeným odkazem se budete moci přihlásit na !login_uri za použití uživatelského jména a hesla, které jste si zvolil/zvolila:

Uživatelské jméno [user:name]
Heslo: heslo, které jste si zvolil/zvolila

Máte-li další otázky, odpovězte na tento e-mail a my se vám budeme snažit pomoci.

Díky,


tým -  !site

--Admin use only
Admin (edit): <a href="[user:edit-url]">[user:edit-url]</a>',
  'user_mail_register_no_approval_required_subject' => '[Warmshowers.org] Ověřte váš nový účet na Warmshowers.org',
  'user_mail_register_pending_approval_body' => 'Welcome [user:name],

*** You must reply to this email to get access to the site! ***
*** Please reply saying something about yourself! ***
*** You must leave the message\'s subject and the rest of this message as they are. ***

( And when you reply - we need you to say "hi". You\'re not replying to a machine but to Randy and the rest of the team! )

*** You won\'t be able to log in until we enable your account
*** which is usually at least several hours after you send your reply.

Thank you for your registration at www.warmshowers.org.

Thank you for applying for an account, and for your hospitality to touring cyclists.

You need to acknowledge that you\'ve received this message by replying to it and saying something about yourself so we can correspond with you and try to make sure everybody on the list is a bona fide member.

I will enable your account after I receive your response.

(Minors - people not of legal age - are not allowed to use the site. So if you are under age, please do not continue.)

I\'ll be happy to approve your account. Your username and password (active after we correspond) are below.

Randy Fay
Warmshowers.org webmaster

AFTER your account has been authorized (after I receive your response and
activate your account), your username and password details will be:

username: [user:name]


AFTER your account is approved, you may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you can always change your password and other account details at  [user:edit-url]

Also: After you log in, please go to "my account" and the "map my location" tab and make sure your location shows correctly on the map. There are instructions on that page that will help you set it.

Thanks for signing up!

-Randy Fay and the registration team
Warmshowers.org

--
Administrator use only:
[user:edit-url]',
  'user_mail_register_pending_approval_subject' => 'Account info for  [user:name] at WarmShowers.org-You must reply and say something about yourself',
  'user_mail_status_activated_body' => 'Vítejte, uživateli [user:name],

Váš účet na www.warmshowers.org byl nyní aktivován.

Můžete se přihlásit svým uživatelským jménem a heslem nebo <a href="[user:one-time-login-url]">kliknutím sem</a>.

Tento odkaz slouží pro jednorázové přihlášení, takže může být použit pouze jednou.

Po přihlášení budete přesměrován na <a href="[user:edit-url]">stránku pro úpravy vašeho profilu</a>, kde můžete své heslo případně změnit.

Pokud mate nějaké otázky, napište je do odpovědi na tento e-mail.

Díky,
-Randy Fay
Warmshowers.org webmaster

--
Admin use only:
!uri',
  'user_mail_status_activated_subject' => '[Warmshowers.org] Váš účet je aktivován.',
  'user_mail_status_blocked_body' => 'Hello [user:name],

Your account on www.warmshowers.org has been blocked.

If you have any questions, please reply to this email.

Thanks,
Randy Fay
Warmshowers.org webmaster

Admin use:
[user:edit-url]',
  'user_mail_status_blocked_subject' => 'Warmshowers.org: [user:name] account blocked',
  'user_mail_status_deleted_body' => 'Uživateli [user:name],

váš účet na www.warmshowers.org byl zrušen, nejspíše na vaši žádost nebo protože byl vadný nebo zneužit ke spamu.

Máte-li nějaké otázky, odpovězte prosím na tento e-mail.

Díky,
Randy Fay
Warmshowers.org',
  'user_mail_status_deleted_subject' => '[Warmshowers.org] Účet uživatele [user:name] zrušen',
  'user_picture_guidelines' => 'Rádi bychom viděli <i>vaši fotografii</i>, třeba na kole nebo ve vašem bydlišti. Není to vůbec povinné, ale bylo by to fajn. Náhodný výběr fotografií se rovněž objevuje na hlavní stránce. <i>Jestliže vložíte fotografii a ta se hned neobjeví, použijte funkci “tvrdé obnovy”. (Stiskněte klávesu SHIFT a klikněte na tlačítko Obnovit (REFRESH) ve vašem prohlížeči.)</i>',
  'user_registration_help' => '<h3> Vítejte! Než budete pokračovat, přečtěte si následující informace</h3>
Těší nás, že se chcete přihlásit do Warm Showers! Vložení informací o vás netrvá dlouho. Poté vám pošleme ověřovací e-mail – musíte kliknout na odkaz v něm uvedený. Ale v současnosti, v miliardách antispamových filtrů, se může stát, že náš e-mail nedostanete, jestliže si nás nepřidáte do vašeho „whitelistu“ (seznam důvěryhodných odesílatelů), a tím by váš účet nikdy nevznikl. Tak tedy…

<ol><li>
Právě teď, před odesláním tohoto formuláře, si přidejte wsl@warmshowers.org na Váš spamový "whitelist" (seznam povolených odesílatelů), nebo nám pošlete e-mail se slovem "test" v předmětu zprávy. Pokud to neuděláte, může se stát, že nedostanete náš validační e-mail s Vaším heslem a registrace nebude dokončena.</li>
<li> Podívejte se prosím do <a href="/the_rules" target="_blank">Pravidel</a>  a zkontrolujte si je, kdykoliv budete na pochybách.</li>
<li> Když se zaregistrujete, uvědomte si, že to znamená, že plánujete poskytovat ubytování, aspoň někdy. Toto je web pro vzájemné ubytovávání. (Pokud jste na cestě nebo z jiného důvodu nemůžete právě teď ubytování poskytovat, označte se zatrhávacím políčkem jako „momentálně nedostupný“ (not currently avaliable); v budoucnosti můžete toto označení zrušit).</li>
<li> Nezletilí (lidé pod zákonnou věkovou hranicí) nesmí tento web používat, takže pokud jste nezletilý, dále již nepokračujte.</li>
<li >Pamatujte, že máme stránku <a href="/faq" target="_blank">FAQ – často kladené dotazy</a> která může vyřešit váš problém, a obraťte se na ni, pokud máte problémy s přihlášením.</li>
<li>Máte obavy z toho, že nevíte, kdo může vidět informace, které sem vložíte? Tento web je konstruován tak, že informace z vašeho profilu nejsou viditelné zvnějšku; mohou je vidět jen členové po svém přihlášení. A vaše e-mailová adresa je vždy skryta, dokonce i jiným členům.</li>
<li>Vaše osobní bezpečnost je pro nás rovněž velmi důležitá. Přečtěte si prosím stránku o <a href="/personal-security">osobní bezpečnosti</a>, kde se dozvíte více.</li>
<li>Naše <a target="_blank" href="/privacy">Zásady ochrany osobních údajů</a>  vás upozorňují, že váš uživatelský profil je viditelný pouze pro ostatní členy a že vás mohou kontaktovat pouze členové.</li>
<li> Registrací souhlasíte s našimi  <a target="_blank" href="/terms">Podmínkami užití</a>, které zdůrazňují, že<strong> pouze vy a vy sám jste odpovědný za všechny interakce se ostatními členy Warmshowers</strong>.
</ol>
<br/><br/>
Dobrá, to je vše! Prosím pokračujte dál a vyplňte vaše registrační údaje.
<hr/>',
  'wsuser_become_available_notification_body' => 'Zdravíme, pane/paní/slečno [user:fullname] - <br/><br/>Váš účet u Warmshowers.org, [user], byl automaticky aktualizován, aby vás zobrazoval jako dostupného hostitele, protože prošlo datum které jste zadal/zadala jako konec vaší nedostupnosti. Pokud jste stále "dočasně nedostupný", přihlaste se prosím na Warmshowers.org a aktualizujte váš účet. Můžete se přihlásit návštěvou stránky nebo jednoduše <a href=\'[user:login_url]\'>kliknutím na tento odkaz</a>. Díky!<br/><br/>-Randy Fay<br/>Warmshowers.org Webmaster<br/>---<br/><a href=\'[user:url]\'>Odkaz na profil</a><br/><a href=\'[account-edit-url]\'>Aktualizace profilu</a><br/><a href=\'[user:login_url]\'>Přímé přihlášení</a><br/>',
  'wsuser_become_available_notification_subject' => 'Nyní jste zařazen do seznamu dostupných hostitelů na Warmshowers.org ([user:fullname])',
  'wsuser_notavailable_reminder_body' => 'Zdravíme, pane/paní/slečno [user:fullname], <br/><br/>váš účet u Warmshowers.org, [user], je označen jako "dočasně nedostupný" pro hostitelství. Můžete se prosím přihlásit a nastavit datum, kdy očekáváte váš návrat? Pokud ho neznáte nebo nemůžete přijímat hosty, vložte vzdálené datum, např. 2025-01-01. To můžete kdykoliv změnit. Můžete se přihlásit uživatelským jménem a heslem na <a href=\'https://www.warmshowers.org\'>www.warmshowers.org</a> nebo <a href=\'[user:login_url]\'>kliknout na tento odkaz pro automatické přihlášení.</a><br/><br/>-Randy Fay<br/>Warmshowers.org Webmaster<br/>---<br/><a href=\'[user:url]\'>Odkaz na profil</a><br/><a href=\'[account-edit-url]\'>Aktualizace profilu</a><br/><a href=\'[user:login_url]\'>Přímé přihlášení</a><br/>',
  'wsuser_notavailable_reminder_subject' => 'Váš účet u Warmshowers je označen jako "dočasně nedostupný". Aktualizujte prosím datum vašeho návratu. ([user:fullname])',
);
  $realm_variables['language']['de'] = array(
  'anonymous' => 'Ehemaliges Mitglied',
  'comment_notify_default_mailtext' => 'Guten Tag [comment-subscribed:author:fullname],

[comment:author:fullname] hat auf "[comment:node:title]" kommentiert:

!commtext

Sie können diesen Kommentar <a href="[comment:url]">hiere</a> sehen und bearbeiten (Bitte antworten Sie nicht via Email, da diese nur an die Administratoren der Website geht.)

Sie können Emailbenachrichtigungen über Antworten auf diesen Post <a href="[comment-subscribed:unsubscribe-url]">hier abbestellen</a>.

Mit freundlichen Grüssen,

das Warmshowers.org-Team',
  'contact_form_information' => 'Schreiben Sie uns eine Mail. Wir antworten so schnell wie möglich.
<p>Sie dürfen uns gerne eine Mail über dieses Formular schreiben.</p>

<p>Falls Sie Hilfe zur Webseite brauchen, <a href="/faq">lesen Sie bitte zuerst die FAQ</a> und konsultieren Sie das <a href="http://de.warmshowers.org/forums/website-help-and-support">Hilfe-Forum</a>, wo Sie Fragen stellen können. Antworten von Mitgliedern können wiederum anderen Mitgliedern Auskunft geben. </p>

<p>Falls Sie englisch sprechen, hilft es den Administratoren sehr, wenn Sie die Frage auf Englisch stellen können. Danke!</p>',
  'faq_description' => '',
  'faq_title' => 'Häufig gestellte Fragen (FAQ)',
  'inactive_user_block_notify_text' => 'Hello %username,

  This automatic message is to notify you that your account on %sitename has been automatically disabled due to no activity for more than %period.

  Please visit us at %siteurl to have your account re-enabled.

Sincerely,
  %sitename team',
  'inactive_user_block_warn_text' => 'Hello %username,

  We haven\'t seen you at %sitename since %lastaccess, and we miss you!  This automatic message is to warn you that your account will be disabled in %period unless you come back and visit us before that time.

  Please visit us at %siteurl.

Sincerely,
  %sitename team',
  'inactive_user_delete_notify_text' => 'Hello %username,

  This automatic message is to notify you that your Warmshowers.org account has been automatically removed because you didn\'t log in for more than %period.

  Please visit us at %siteurl if you would like to create a new account.

Sincerely,
-Randy Fay
Warmshowers.org Webmaster',
  'inactive_user_delete_warn_text' => 'Hello %username,

Your account at Warmshowers.org will soon be removed (in %period) unless you log in to let us know you still want it. Please log into the site at http://www.warmshowers.org and update your account.  If you don\'t know your username or password that\'s OK - you can log in with your email address. If you don\'t know your password, please visit http://www.warmshowers.org/user/password to get a one-time login link emailed to you.

You haven\'t logged in %lastaccess.   This automatic message is to warn you that your account will be completely removed in %period unless you log into the site before that time.

  Please visit us at %siteurl.

Sincerely,
-Randy Fay
Warmshowers.org webmaster',
  'inactive_user_notify_text' => 'Hallo %username,

Sie haben sich seit %lastaccess nicht bei Warmshowers eingeloggt und wir vermissen Sie! Wir bitten alle, sich mindestens jährlich einzuloggen, um alle Informationen auf ihre Aktualität zu überprüfen. Deshalb würden wir es schätzen, wenn Sie sich einloggen und ihr Konto aktualisieren oder zumindest einfach einloggen, damit das System weiss, dass Ihre Mailadresse noch stimmt.

Bitte aktualisieren Sie Ihr Profil bald: https://de.warmshowers.org.  Falls Sie sich länger als ein Kalenderjahr nicht einloggen, wird Ihr Profil gelöscht! (aber Sie erhalten zuerst mehrere Warnungen)

Falls Sie Ihr Passwort vergessen haben, gehen Sie einfach auf http://de.warmshowers.org/user/password und geben Sie Ihre Mailadresse ein (%useremail). Dann erhalten Sie einen einmalig gültigen Login-Link an Ihre Mailadresse gesandt.

Mit besten Grüssen,
Randy Fay
Warmshowers.org webmaster

------
Admin Informationen
Suche: http://www.warmshowers.org/search/wsuser/%useremail
Benutzername: %username
Mailadresse: %useremail',
  'node_notify_default_mailtext' => 'Guten Tag [comment:node:author:fullname],

Sie haben einen Kommentar auf "[comment:node:title]" erhalten:

!commtext

Sie können die Kommentare <a href="[comment:url]">hier sehen</a> (bitte antworten Sie nicht via Email, da diese nur an die Administratoren der Website geht).

Sie werden solche Emails auf alle Antworten auf Ihre Posts erhalten.

Mit freundlichen Grüssen,

das Warmshowers.org-Team',
  'site_footer' => ' Gastfreundschaft für Tourenradfahrer<br/>
<a href="/">Startseite</a>',
  'site_mission' => '<a href="http://de.warmshowers.org">warmshowers.org</a>: Gastfreundschaft für Tourenradfahrer',
  'site_name' => 'Warm Showers',
  'site_slogan' => 'Gastfreundschaft für Tourenradfahrer',
  'user_mail_password_reset_body' => 'Hallo [user:name],

Sie (oder sonst jemand) haben einen einmaligen Login-Link für Ihr Konto auf warmshowers.org angefordert. Falls Sie dies nicht waren, beachten Sie diese Mail als gegenstandslos.

Klicken Sie bitte <a href="[user:one-time-login-url]">hier, um sich einzuloggen und das Passwort zurückzusetzen</a>. Falls Sie nicht auf diesen Link nicht öffnen können, kopieren Sie diesen Link und fügen Sie ihn in Ihrem Browser ein:

[user:one-time-login-url]

Dies ist ein einmaliger Login-Link, der (logischerweise) nur einmal gebraucht werden kann. Er läuft nach einem Tag ab, wonach nichts passieren wird, wenn er nicht gebraucht wird.

Nachdem Sie sich eingeloggt haben, werden Sie auf <a href="[user:edit-url]">Ihre \'\'Profil bearbeiten\'\'-Seite</a> weitergeleitet, damit Sie das Passwort ändern können.

Vielen Dank,
-Randy Fay und das Warmshowers-Team.
WarmShowers.org

Fehlersuche:

Falls Sie auf "Entschuldigung - Sie haben keinen Zugriff auf diese Seite (Fehler 403)" kommen ist es fast sicher, dass Sie Cookies in Ihrem Browser aktivieren müssen. Sehen Sie sich dazu <a href="http://de.warmshowers.org/content/ich-will-mich-einloggen-aber-bekomme-zugriff-verweigert">diesen Artikel in den FAQ an</a>.

Falls Sie das Problem nicht lösen können, fragen Sie bitte nach Hilfe, indem Sie auf diese Mail antworten.',
  'user_mail_password_reset_subject' => '[Warmshowers.org] Einmaliger Login-Link für [user:fullname] ([user:name])',
  'user_mail_register_admin_created_body' => '[user:name],

A site administrator at !site has created an account for you. You may now log in to !login_uri using the following username and password:

username: [user:name]


You may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you will be redirected to [user:edit-url] so you can change your password.


--  !site team',
  'user_mail_register_admin_created_subject' => 'An administrator created an account for you at  !site',
  'user_mail_register_no_approval_required_body' => '[user:name],

Danke für die Registrierung auf Warmshowers.org.

Bitte <a href="[user:one-time-login-url]">klicken Sie hier,</a> um Ihre Mailadresse zu bestätigen und Ihr Konto zu aktivieren. (Falls Sie nicht auf diesen Link klicken können, fügen Sie bitte diesen Link in Ihren Browser ein:

[user:one-time-login-url]

Nachdem Sie Ihr Konto mit dem obenstehenden Link validiert haben, können Sie sich einloggen in !login_uri mittels dem folgenden Benutzernamen und dem Passwort, das Sie eingegeben haben:

Benutzername: [user:name]
Passwort: Das Passwort, das Sie eingegeben haben

Falls Sie irgendwelche Fragen haben, können Sie auf diese Mail antworten und wir tun unser Bestes, um zu helfen.

Vielen Dank,

--  das !site Team

--Nur für Administratoren:
Admin (bearbeiten): <a href="[user:edit-url]">[user:edit-url]</a>',
  'user_mail_register_no_approval_required_subject' => '[Warmshowers.org] Aktivieren Sie Ihr Konto auf Warmshowers.org ([user:name])',
  'user_mail_register_pending_approval_body' => 'Welcome [user:name],

*** You must reply to this email to get access to the site! ***
*** Please reply saying something about yourself! ***
*** You must leave the message\'s subject and the rest of this message as they are. ***

( And when you reply - we need you to say "hi". You\'re not replying to a machine but to Randy and the rest of the team! )

*** You won\'t be able to log in until we enable your account
*** which is usually at least several hours after you send your reply.

Thank you for your registration at www.warmshowers.org.

Thank you for applying for an account, and for your hospitality to touring cyclists.

You need to acknowledge that you\'ve received this message by replying to it and saying something about yourself so we can correspond with you and try to make sure everybody on the list is a bona fide member.

I will enable your account after I receive your response.

(Minors - people not of legal age - are not allowed to use the site. So if you are under age, please do not continue.)

I\'ll be happy to approve your account. Your username and password (active after we correspond) are below.

Randy Fay
Warmshowers.org webmaster

AFTER your account has been authorized (after I receive your response and
activate your account), your username and password details will be:

username: [user:name]


AFTER your account is approved, you may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you can always change your password and other account details at  [user:edit-url]

Also: After you log in, please go to "my account" and the "map my location" tab and make sure your location shows correctly on the map. There are instructions on that page that will help you set it.

Thanks for signing up!

-Randy Fay and the registration team
Warmshowers.org

--
Administrator use only:
[user:edit-url]',
  'user_mail_register_pending_approval_subject' => 'Account info for  [user:name] at WarmShowers.org-You must reply and say something about yourself',
  'user_mail_status_activated_body' => 'Willkommen [user:name],

Ihr Konto auf www.warmshowers.org wurde aktiviert.

Sie können sich mit dem Benutzernamen und dem Passwort das Sie bereits haben einloggen oder indem Sie <a href="[user:one-time-login-url]">hier</a> klicken.

Dies ist ein einmaliger Login-Link. Nachdem Sie sich eingeloggt haben, werden Sie auf die <a href="[user:edit-url]">\'\'Profil bearbeiten\'\'-Seite</a> weitergeleitet, damit Sie, falls Sie wollen, Ihr Passwort ändern können.

Irgendwelche Fragen? Antworten Sie einfach auf diese Nachricht.

Sportliche Grüsse,
Randy Fay
Warmshowers.org webmaster

--
Nur für Administratoren:
!uri',
  'user_mail_status_activated_subject' => '[Warmshowers.org] Ihr Konto wurde aktiviert',
  'user_mail_status_blocked_body' => 'Hallo [user:name],

Ihr Konto auf www.warmshowers.org wurde gesperrt.

Falls Sie fragen haben, antworten Sie auf diese Mail.

Freundliche Grüsse,
Randy Fay
Warmshowers.org webmaster

Nur für Administratoren:
[user:edit-url]',
  'user_mail_status_blocked_subject' => 'Warmshowers.org: Ihr Konto ([user:name]) wurde gesperrt',
  'user_mail_status_deleted_body' => 'Hallo [user:name],

Ihr Konto auf www.warmshowers.org wurde gelöscht, höchstwahrscheinlich auf Ihren Wunsch oder da es sich um ein ungültiges Spam-Konto handelte.

Falls Sie fragen haben, antworten Sie auf diese Mail.

Freundliche Grüsse,
Randy Fay
Warmshowers.org',
  'user_mail_status_deleted_subject' => '[Warmshowers.org] Ihr Kont ([user:name]) wurde gelöscht',
  'user_picture_guidelines' => 'Wir würden uns über ein Bild von <i>Ihnen</i> freuen, vielleicht eines, wo Sie in Ihrer Gegend Fahrrad fahren. Dies ist natürlich völlig freiwillig aber kann unterhaltsam sein. <i>Falls Sie ein neues Bild hochladen und es nicht sofort angezeigt wird, aktualisieren Sie die Seite in Ihrem Browser.</i>
',
  'user_registration_help' => '<h3>Willkommen! Bitte lesen Sie dies, bevor Sie unten fortfahren</h3>
Wir sind froh, dass Sie sich bei Warmshowers anmelden wollen! In einer Mitnute können Sie all Ihre Informationen eingeben, wonach wir Ihnen eine Bestätigungsmail senden. Danach müssen Sie auf den Link in dieser Mail klicken. Vielleicht müssen Sie uns zuerst zu Ihrer \'\'Liste vertrauenswürdiger Absender\'\' (oder ähnlich) hinzufügen, damit unsere Mails nicht im Spam-Ordner landen, oder Sie senden uns einfach eine Mail mit dem Betreff "test".
<ol>
<li>Bitte schauen Sie sich <a href="/the_rules" target="_blank">die Regeln</a> an und tun Sie dies erneut, wann immer Sie Zweifel haben.</li>
<li>Wenn Sie sich anmelden, sollten Sie im Sinne haben, zumindest eines Tages Beherbergung anzubieten. Dies ist eine Website für gegenseitige Beherbergung. (Falls Sie auf einer Tour sind oder sonst keine Beherbergung anbieten können, klicken Sie auf die Box \'\'Zurzeit nicht verfügbar\'\' und wählen Sie die Option wieder ab, sobald Sie beherbergen können. </li>
<li>Minderjährige (Leute unter 18 Jahren) haben keinen Zugriff auf die Website.</li>
<li>Wir haben eine Seite mit <a href="/faq" target="_blank">häufig gestellten Fragen (FAQ)</a>, welche einige Probleme lösen hilft und Ihnen hilft, falls Sie jemals Probleme beim Login haben sollten.</li>
<li>Sind Sie besorgt darum, wer Ihre Informationen sehen kann? Die Website ist so gemacht, dass die Informationen Ihres Profils nicht sichtbar sind für die Außenwelt. Nur eingeloggte Mitglieder werden Ihre Informationen sehen können. Ihre Mailadresse ist immer versteckt, auch für Mitglieder. </li>
<li>Ihre Privatsicherheit ist uns sehr wichtig. Lesen Sie bitte <a href="/is_this_safe">diese Seite</a> für mehr Informationen dazu.</li>
<li>Unsere <a target="_blank" href="/privacy">Datenschutzerklärungen</a> erinnern Sie, dass Ihr Mitgliedsprofil nur für andere Mitglieder sichtbar ist und dass nur Mitglieder Sie kontaktieren können.</li>
<li>Indem Sie sich anmelden stimmen Sie den <a target="_blank" href="/terms">Nutzungsbedingungen</a> zu, was hervorhebt, das <strong>Sie alleine verantwortlich sind für jegliche Interaktionen mit Warmshowers-Mitgliedern.</strong></li>
</ol><br/><br/>
O.K., das ist alles! Bitte fahren Sie unten mit Ihrer Registrierung fort.
<hr/>',
  'wsuser_become_available_notification_body' => 'Guten Tag [user:fullname] - <br/><br/>Ihr Warmshowers.org-Konto [user] wurde automatisch aktualisiert, so dass Sie als "zurzeit verfügbar" für Beherbergungen angezeigt werden, da das Datum, an dem Sie angegeben haben wieder verfügbar zu sein, überschritten wurde. Falls Sie immer noch nicht verfügbar sein sollten, loggen Sie sich bitte auf Warmshowers.org ein und aktualisieren Sie Ihre Benutzerdaten. Sie können sich einloggen, indem Sie die Seite besuchen oder <a href=\'[user:login_url]\'>hier klicken</a>. Vielen Dank!<br/><br/>-Randy Fay<br/>Warmshowers.org Webmaster<br/>---<br/><a href=\'[user:url]\'>Profil-Link</a><br/><a href=\'[account-edit-url]\'>Profil-beareiten-Link</a><br/><a href=\'[user:login_url]\'>Direkter Login-Link</a><br/>',
  'wsuser_become_available_notification_subject' => 'Sie sind nur als verfügbar für Beherbungen auf Warmshowers.org eingetragen ([user:fullname])',
  'wsuser_notavailable_reminder_body' => 'Guten Tag [user:fullname] <br/><br/>Ihr Warmshowers-Konto [user] ist als "zurzeit nicht verfügabar" markiert. Könnten Sie sich bitte einloggen und das Datum eintragen, an dem Sie zurückzukommen gedenken? Falls Sie noch nicht wissen wann Sie wieder verfügbar sein werden, tragen Sie bitte einfach ein weit entferntes Datum wie 2025-01-01 ein. Sie können dies jederzeit ändern. Sie können sich mit ihrem Benutzernamen und Ihrem Passwort auf <a href=\'https://de.warmshowers.org\'>de.warmshowers.org</a> oder <a href=\'[user:login_url]\'>indem Sie hier klicken</a> einloggen.<br/><br/>-Randy Fay<br/>Warmshowers.org Webmaster<br/>---<br/><a href=\'[user:url]\'>Profil-Link</a><br/><a href=\'[account-edit-url]\'>Profil bearbeiten</a><br/><a href=\'[user:login_url]\'>Direkter Login-Link</a><br/>',
  'wsuser_notavailable_reminder_subject' => 'Ihr Warmshowers-Konto ist als zurzeit nicht verfügbar markiert. Bitte geben Sie Ihre Rückkehr an  ([user:fullname])',
);
  $realm_variables['language']['en'] = array();
  $realm_variables['language']['en-working'] = array(
  'anonymous' => 'Former member',
  'comment_notify_default_mailtext' => 'Hi [comment-subscribed:author:fullname],

[comment:author:fullname] has commented on: "[comment:node:title]":

---- [comment:title] [comment:body] ----

You can view and reply to the comment by <a href="[comment:url]">clicking here</a>. (Please don\'t respond via email as the email just goes to the site managers.)

You can stop receiving emails when someone replies to this post by <a href="[comment-subscribed:unsubscribe-url]">clicking here</a>.

- Warmshowers.org team',
  'contact_form_information' => '<p>You are welcome to contact us here via email.</p>

<p>If you need help using the site, please <a href="/faq">read the FAQ first</a>, and then please consider reading the <a href="http://www.warmshowers.org/forums/website-help-and-support">Help Forum</a> and posting there where others can help you, and the answer will help others as well.</p>

<p>If you speak English it will help the admins a *lot* if you can ask your question in English. Thanks! </p>

<p>Full Contact Info: Warmshowers.org Foundation, 848 Montclair Drive, Palisade, CO 81526, USA. Phone: +1 970.462.7450. Email: wsl@warmshowers.org</p>',
  'faq_description' => '',
  'faq_title' => 'Frequently Asked Questions',
  'inactive_user_block_notify_text' => 'Hello %username,

  This automatic message is to notify you that your account on %sitename has been automatically disabled due to no activity for more than %period.

  Please visit us at %siteurl to have your account re-enabled.

Sincerely,
  %sitename team',
  'inactive_user_block_warn_text' => 'Hello %username,

  We haven\'t seen you at %sitename since %lastaccess, and we miss you!  This automatic message is to warn you that your account will be disabled in %period unless you come back and visit us before that time.

  Please visit us at %siteurl.

Sincerely,
  %sitename team',
  'inactive_user_delete_notify_text' => 'Hello %username,

  This automatic message is to notify you that your Warmshowers.org account has been automatically removed because you didn\'t log in for more than %period.

  Please visit us at %siteurl if you would like to create a new account.

Sincerely,
-Randy Fay
Warmshowers.org Webmaster',
  'inactive_user_delete_warn_text' => 'Hello %username,

Your account at Warmshowers.org will soon be removed (in %period) unless you log in to let us know you still want it. Please log into the site at http://www.warmshowers.org and update your account.  If you don\'t know your username or password that\'s OK - you can log in with your email address. If you don\'t know your password, please visit http://www.warmshowers.org/user/password to get a one-time login link emailed to you.

You haven\'t logged in %lastaccess.   This automatic message is to warn you that your account will be completely removed in %period unless you log into the site before that time.

  Please visit us at %siteurl.

Sincerely,
-Randy Fay
Warmshowers.org webmaster',
  'inactive_user_notify_text' => 'Hello %username,

You haven\'t logged into Warmshowers.org since %lastaccess, and we miss you!  We ask everyone to log in at least annually to make sure their information is correct, so we\'d appreciate it if you\'d log in and update your account. Or at least *log in* so that the system knows you\'re still getting email at this address.

Please log in soon at https://www.warmshowers.org.  If you do not log in for a full calendar year your account will eventually be deleted (but you\'ll receive more than one warning first).

If you have forgotten your password, just go to http://www.warmshowers.org/user/password and put in your email address (%useremail) and you\'ll get a one-time login link in your email.

Sincerely,
-Randy Fay
Warmshowers.org webmaster

------
Admin Information
Search: http://www.warmshowers.org/search/wsuser/%useremail
Username: %username
Email: %useremail',
  'node_notify_default_mailtext' => 'Hi [comment:node:author:fullname],

You have received a comment on: "[comment:node:title]":

----
[comment:title]
[comment:body]
----

You can view the comment by <a href="[comment:url]">clicking here</a>.  (Please don\'t respond via email as the email just goes to the site managers.)

You will receive emails like this for all replies to your posts.

- Warmshowers team',
  'site_footer' => 'Hospitality for Touring Bicyclists Worldwide<br/>
<a href="/welcome">Home</a>  <a href="/map">Map</a>',
  'site_mission' => 'the members of <a href="http://warmshowers.org">warmshowers.org</a>: hospitality for touring cyclists',
  'site_name' => 'Warmshowers.org',
  'site_slogan' => 'Hospitality for touring cyclists',
  'uc_cart_breadcrumb_text' => '',
  'uc_cart_new_account_details' => '<b>Optional.</b> New customers may supply custom account details.<br />We will create these for you if no values are entered.',
  'uc_checkout_instructions' => '',
  'uc_checkout_review_instructions' => '',
  'uc_check_policy' => 'Personal and business checks will be held for up to 10 business days to ensure payment clears before an order is shipped.',
  'uc_cod_policy' => 'Full payment is expected upon delivery or prior to pick-up.',
  'uc_continue_shopping_text' => '',
  'uc_credit_fail_message' => 'We were unable to process your payment. Please verify your details and try again.  If the problem persists, contact us for help on the <a href="/contact">contact form</a>. Thanks so much for doing this, and thanks for your (future) donation!',
  'uc_credit_policy' => 'Most debit and credit cards will work fine.',
  'uc_default_payment_msg' => '',
  'uc_msg_continue_shopping' => '<a href="[site-url]">Return to the Warmshowers.org dashboard.</a>',
  'uc_msg_order_existing_user' => 'Thank you so much for your generous contribution!',
  'uc_msg_order_logged_in' => '',
  'uc_msg_order_new_user' => 'Thank you for shopping at [store-name]. A new account has been created for you here that you may use to view your current order status.

<a href="https://www.warmshowers.org/user">Login</a> to your new account using the following information:

<strong>Username:</strong> !new_username
<strong>Password:</strong> !new_password',
  'uc_msg_order_new_user_logged_in' => 'Thank you for shopping at [store-name]. A new account has been created for you here that you may use to view your current order status.

Your password and further instructions have been sent to your e-mail address.

For your convenience, you are already logged in with your newly created account.',
  'uc_msg_order_submit' => 'Thanks so much for your donation of [order-total]. You have made a real difference to our community! Your membership will last for the coming year.',
  'uc_paypal_wps_checkout_button' => 'Send Your Donation',
  'uc_product_add_to_cart_text' => 'Give now!',
  'uc_roles_default_expiration_header' => 'Your membership is expiring',
  'uc_roles_default_expiration_message' => 'Your membership will expire on !date',
  'uc_roles_default_expiration_title' => '!role_name',
  'uc_store_name' => 'Warmshowers.org Donations',
  'uc_teaser_add_to_cart_text' => 'Give now!',
  'user_mail_password_reset_body' => 'Hi [user:name],

You (or someone) requested a one-time login for your account on warmshowers.org. If it wasn\'t you, you can just ignore this message.

Please <a href="[user:one-time-login-url]">click here to log in and reset your password</a>. If you can\'t click that link, copy and paste this URL into your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once. It expires after one day and nothing will happen if it\'s not used.

After logging in, you will be redirected to <a href="[user:edit-url]">your account profile edit page</a> so you can change your password.

Thanks,
-Randy Fay and the registration team
WarmShowers.org

Troubleshooting:

If you get "Sorry - you don\'t have access to that page (error 403)", it almost certainly means you have cookies turned off on your browser.  To get cookies configured correctly please see <a href="http://www.warmshowers.org/content/i-try-log-it-still-says-im-not-logged">this article in the FAQ</a>.

If you can\'t solve this problem, please ask for help. Reply to this message and ask for help.',
  'user_mail_password_reset_subject' => '[Warmshowers.org] One-time login link for  [user:name]',
  'user_mail_register_admin_created_body' => '[user:name],

A site administrator at !site has created an account for you. You may now log in to !login_uri using the following username and password:

username: [user:name]


You may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you will be redirected to [user:edit-url] so you can change your password.


--  !site team',
  'user_mail_register_admin_created_subject' => 'An administrator created an account for you at  !site',
  'user_mail_register_no_approval_required_body' => '[user:name],

Thank you for registering at Warmshowers.org.

Please <a href="[user:one-time-login-url]">click here</a> to validate your email address and enable your account. (If you can\'t click that link, please copy and paste this link into your browser:

[user:one-time-login-url]

After you validate with the link above you will be able to log in to !login_uri using the following username and the password you entered:

username: [user:name]
password: The password you entered

If you have any questions, you can reply to this email and we\'ll do our best to help you out.

Thanks,


--  !site team

--Admin use only
Admin (edit): <a href="[user:edit-url]">[user:edit-url]</a>',
  'user_mail_register_no_approval_required_subject' => '[Warmshowers.org] Validate your account on Warmshowers.org ([user:name])',
  'user_mail_register_pending_approval_body' => 'Welcome [user:name],

*** You must reply to this email to get access to the site! ***
*** Please reply saying something about yourself! ***
*** You must leave the message\'s subject and the rest of this message as they are. ***

( And when you reply - we need you to say "hi". You\'re not replying to a machine but to Randy and the rest of the team! )

*** You won\'t be able to log in until we enable your account
*** which is usually at least several hours after you send your reply.

Thank you for your registration at www.warmshowers.org.

Thank you for applying for an account, and for your hospitality to touring cyclists.

You need to acknowledge that you\'ve received this message by replying to it and saying something about yourself so we can correspond with you and try to make sure everybody on the list is a bona fide member.

I will enable your account after I receive your response.

(Minors - people not of legal age - are not allowed to use the site. So if you are under age, please do not continue.)

I\'ll be happy to approve your account. Your username and password (active after we correspond) are below.

Randy Fay
Warmshowers.org webmaster

AFTER your account has been authorized (after I receive your response and
activate your account), your username and password details will be:

username: [user:name]


AFTER your account is approved, you may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you can always change your password and other account details at  [user:edit-url]

Also: After you log in, please go to "my account" and the "map my location" tab and make sure your location shows correctly on the map. There are instructions on that page that will help you set it.

Thanks for signing up!

-Randy Fay and the registration team
Warmshowers.org

--
Administrator use only:
[user:edit-url]',
  'user_mail_register_pending_approval_subject' => 'Account info for  [user:name] at WarmShowers.org-You must reply and say something about yourself',
  'user_mail_status_activated_body' => 'Welcome [user:name],

Your account at www.warmshowers.org has been activated.

You can log in with the username and password you already have, or you may now log in by <a href="[user:one-time-login-url]">clicking here</a>.

This is a one-time login, so it can be used only once.

After logging in, you will be redirected to <a href="[user:edit-url]">your account profile edit page</a> so you can change your password if you want to.

Any questions? Just reply to this message.

Thanks,
-Randy Fay
Warmshowers.org webmaster

--
Admin use only:
!uri',
  'user_mail_status_activated_subject' => '[Warmshowers.org] Your account has been approved',
  'user_mail_status_blocked_body' => 'Hello [user:name],

Your account on www.warmshowers.org has been blocked.

If you have any questions, please reply to this email.

Thanks,
Randy Fay
Warmshowers.org webmaster

Admin use:
[user:edit-url]',
  'user_mail_status_blocked_subject' => 'Warmshowers.org: [user:name] account blocked',
  'user_mail_status_deleted_body' => 'Hello [user:name],

Your account on www.warmshowers.org has been deleted, most likely at your request or as a result of appearing to be an invalid or spammer entry.

If you have any questions, please reply to this email.

Thanks,
Randy Fay
Warmshowers.org',
  'user_mail_status_deleted_subject' => '[Warmshowers.org] [user:name] account deleted',
  'user_picture_guidelines' => 'We\'d like a picture of <i>you</i>, perhaps cycling or at your location. This is entirely optional, of course, but can be fun and helps guests or hosts know more about you. Random pictures show on the front page as well. <i>If you upload a new picture and it doesn\'t show up right away, do a shift-refresh on your browser. (Hold down the SHIFT key and click the REFRESH button on your browser.)</i>
',
  'user_registration_help' => '<h3>Welcome! Please read this before continuing below</h3>
We\'re glad you want to sign up for Warm Showers! In a minute we\'re going to have you enter all your information, and then we\'ll send you a validation email - you have to click a link in that email. But of course in these days of billions of spam filters, you might not get the email if you don\'t add us to your spam whitelist, and then your account will never be enabled. So....

<ol><li>
Right now, before submitting this form, please add wsl@warmshowers.org to your Spam whitelist (or just send us a quick email with "test" as the subject). If you don\'t do this, you may not receive the validation email from us with your password, so you won\'t get signed up.</li>
<li>Please take a look at <a href="https://www.warmshowers.org/faq#t27n4029" target="_blank">the Rules</a> and remember to check them out whenever you are in doubt.</li>
<li>When you sign up, please remember that it means you\'re planning to offer hospitality, at least someday. This is a reciprocal hospitality site. (If you are on tour or otherwise can\'t offer hospitality right now, you\'ll check the "not currently available" checkbox and then in the future you can uncheck it in your account.)</li>
<li>Minors (people not of legal age) are not allowed to use the site, so please do not continue if you are a minor.</li>
<li>Remember that we have a <a href="/faq" target="_blank">Frequently Asked Questions</a> page, which may resolve problems if you have them, and remember to check it if you ever have trouble logging into the site.</li>
<li>Are you concerned about who can see the information you put here?  The website is constructed such that the information in your profile is not visible to the outside world; only members will be able to view it after they have logged in.  And your email address is always hidden, even from other members.</li>
<li>Your personal safety is also very important to us.  Please read the <a href="/personal-security">the Personal Security page</a> for more information on this subject.</li>
<li>Our <a target="_blank" href="/privacy">Privacy Policy</a> reminds you that your member profile is visible only to other members and that only members can contact you.</li>
<li>By signing up you agree to our <a target="_blank" href="/terms">Terms of Use</a> which emphasize that <strong>you and you alone are responsible for every interaction with other Warmshowers members</strong></li>
</ol>
<br/><br/>
OK, that\'s all! Please continue on below and fill in your registration.
<hr/>',
  'wsuser_become_available_notification_body' => 'Hi [user:fullname] - <br/><br/>Your Warmshowers.org account, [user], has been automatically updated to show you as "available to host" because the date that you specified you would become available after has passed. If you are still unavailable to host please log into Warmshowers.org and update your account. You can log in by visiting the site or just <a href=\'[user:login_url]\'>clicking this link</a>. Thanks!<br/><br/>-Randy Fay<br/>Warmshowers.org Webmaster<br/>---<br/><a href=\'[user:url]\'>Profile link</a><br/><a href=\'[account-edit-url]\'>Edit profile link</a><br/><a href=\'[user:login_url]\'>Direct login link</a><br/>',
  'wsuser_become_available_notification_subject' => 'You are now listed as available to host on Warmshowers.org ([user:fullname])',
  'wsuser_notavailable_reminder_body' => 'Hi [user:fullname], <br/><br/>Your warmshowers.org account, [user:name], is marked as unavailable to host. Could you please log in and set the date that you expect to return? If you don\'t know when or if you\'ll be able to host, just put in a date like 2025-01-01. You can change it at any time. You can log in with your username and password at <a href=\'https://www.warmshowers.org\'>www.warmshowers.org</a> or <a href=\'[user:login_url]\'>click this link to log in automatically</a><br/><br/>-Randy Fay<br/>Warmshowers.org Webmaster<br/>---<br/><a href=\'[user:url]\'>Profile link</a><br/><a href=\'[account-edit-url]\'>Edit profile link</a><br/><a href=\'[user:login_url]\'>Direct login link</a><br/>',
  'wsuser_notavailable_reminder_subject' => 'junk [user:fullname]',
);
  $realm_variables['language']['es'] = array(
  'anonymous' => 'Anonymous',
  'comment_notify_default_mailtext' => 'Hola [comment-subscribed:author:fullname],

[comment:author:fullname] ha comentado sobre: "[comment:node:title]":

----
[comment:title]
[comment:body]
----

Usted puede ver y contestar por <a href="[comment:url]">cliquear aquí</a>. Favor de no responder por correo electrónico porque el correo va solamente a los manageros del sitio.

Usted puede dejar de recibir correos electrónicos de notificación cuando alguién responde a este por <a href="[comment-subscribed:unsubscribe-url]">cliquear aquí</a>.

- Equipo de Warmshowers.org',
  'contact_form_information' => '<p>Puedes contactar con nosotros aquí.</p>
<p>Si necesitas ayuda para utilizar la página, primero lee <a href="/faq">las preguntas frecuentes (FAQ)</a>, y si es necesario <a href="http://www.warmshowers.org/forums/website-help-and-support">en el foro de ayuda</a> puedes escribir tus dudas, y otros usuarios podrán ayudarte (o tú ayudar a otros).</p>
<p>If you speak English it will help the admins a *lot* if you can ask your question in English. Thanks! </p>',
  'node_notify_default_mailtext' => 'Hola [comment:node:author:fullname],

Usted ha recibido un comento en "[comment:node:title]":

----
[comment:title]
[comment:body]
----

Se puede ver este comento por <a href="[comment:url]">cliquear aquí</a>.  (Favor de no responder por correo electrónico porque el correo va solamente a los manageros del sitio.)

- Equipo de Warmshowers.org',
  'site_footer' => 'Hospedaje para cicloturistas en todo el mundo<br/>
<a href="/">Inicio</a>  <a href="/map">Mapa</a>',
  'site_mission' => 'los miembros de <a href="http://es.warmshowers.org">warmshowers.org</a>: hospitalidad entre cicloturistas',
  'site_name' => 'Duchas Calientes (Warm Showers)',
  'site_slogan' => 'Hospitalidad entre cicloturistas',
  'user_mail_password_reset_body' => 'Hola [user:name]:

Al parecer has solicitado que se restablezca la contraseña de tu cuenta de Warmshowers.org. Si no fuiste tú, puedes simplemente ignorar este mensaje.

Ahora puedes acceder a tu cuenta <a href="[user:one-time-login-url]">haciendo click en este enlace</a>.

Este acceso funciona solamente una vez y durante un día desde que te lo mandamos. Si no lo usas, no pasará absolutamente nada.

Al acceder a tu cuenta serás redirigido a <a href="[user:edit-url]">perfil de su cuenta la página de edición</a> para que puedas cambiar tu contraseña.

Gracias.

El equipo de registro de Warmshowers.org


Solución de problemas:

Si te si te aparece el mensaje "Acceso denegado. No estás autorizado a acceder a esta página", seguramente las cookies de tu navegador están desactivadas. En el apartado "Preguntas frecuentes (FAQ)" te explicamos cómo activarlas:

<i>Hago click en el enlace para restablecer la contraseña y me aparece "acceso denegado"</i>

Normalmente el problema consiste en que las cookies de tu navegador están desactivadas. Para acceder a tu cuenta debes habilitar las "cookies". La mayoría de los sitios web necesitan las cookies para funcionar correctamente. No te preocupes por ellas, no son peligrosas. Para habilitar las cookies puedes probar alguna de las siguientes soluciones:

<i>Si usas Internet Explorer:</i>
    * Ve a Herramientas > Opciones de Internet > Privacidad, y selecciona cualquier opción excepto "Bloquear todas las cookies".
    * Haz click en "sitios" en la misma ventana de opciones y habilita las cookies para warmshowers.org.
    * En algunos casos Internet Explorer no funciona correctamente, y para solucionarlo debes borrar todas las cookies (Herramientas > Opciones de Internet > General > Eliminar Cookies).
    * Asegúrate de que tu caché esté limpio. En Internet Explorer 6 Herramientas > Opciones de Internet > General > "Eliminar archivos".
    * En Internet Explorer 7 puedes solucionar la mayoria de los problemas en Herramientas > Opciones de Internet > Opciones Avanzadas, haciendo click en "Restaurar valores predeterminados".

De cualquier manera, te recomendamos utilizar una alternativa a otro navegador de internet ,como <a href="http://getfirefox.com">Firefox</a>. Si no puedes instalar Firefox porque no tienes derechos de administrador en el equipo, puedes probar su versión portátil, que no requiere instalación: <a href="http://portableapps.com/apps/internet/firefox_portable">Firefox Portable Edition</a> (no está disponible en castellano). (Randy lleva este programa en su memoria USB cuando sale de viaje en bicicleta para evitar este tipo de problemas.)

<i>Si usas Firefox:</i>
    * Ve a Herramientas > Opciones > Privacidad y activa "Aceptar cookies de las webs".
    * Asegúrate de que tu caché esté limpio: En Herramientas > Limpiar datos privados (o directamente Ctrl+Mayús+Supr), comprueba que estén marcados "Caché" y "Sesiones activas" y haz click en "Limpiar datos privados ahora".

Este problema puede causarlo un programa que bloquee las cookies. Por ejemplo, CA Security Center puede bloquear las cookies en todos los navegadores. Intenta como último recurso deshabilitar temporalmente el firewall y el antivirus, para ver si son la causa del problema.

Si no puedes resolver este problema, responde a este mensaje e intentaremos ayudarte.',
  'user_mail_password_reset_subject' => '[Warmshowers.org] Enlace de un solo uso para restablecer la contraseña de [user:name]',
  'user_mail_register_admin_created_body' => '[user:name],

A site administrator at !site has created an account for you. You may now log in to !login_uri using the following username and password:

username: [user:name]


You may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you will be redirected to [user:edit-url] so you can change your password.


--  !site team',
  'user_mail_register_admin_created_subject' => 'An administrator created an account for you at  !site',
  'user_mail_register_no_approval_required_body' => 'Hola [user:name]:

Gracias por registrarte en Warmshowers.org.

Por favor <a href="[user:one-time-login-url]">click aqui</a> para validar tu dirección de correo electrónico y habilitar tu cuenta.

[user:one-time-login-url]

Después de hacerlo, podrás acceder a tu cuenta en !login_uri usando los siguientes nombre de usuario y contraseña (que podrás cambiar después):

usuario: [user:name]

(Puedes usar tu correo electrónico en vez de nombre de usuario, también.)

Si tienes alguna pregunta, puedes contestar a este correo y haremos lo que podamos para ayudarte.

Gracias.

Warmshowers.org administración

Uso admin (edit): <a href="[user:edit-url]">[user:edit-url]</a>',
  'user_mail_register_no_approval_required_subject' => 'Valida tu cuenta en Warmshowers.org ([user:name])',
  'user_mail_register_pending_approval_body' => 'Welcome [user:name],

*** You must reply to this email to get access to the site! ***
*** Please reply saying something about yourself! ***
*** You must leave the message\'s subject and the rest of this message as they are. ***

( And when you reply - we need you to say "hi". You\'re not replying to a machine but to Randy and the rest of the team! )

*** You won\'t be able to log in until we enable your account
*** which is usually at least several hours after you send your reply.

Thank you for your registration at www.warmshowers.org.

Thank you for applying for an account, and for your hospitality to touring cyclists.

You need to acknowledge that you\'ve received this message by replying to it and saying something about yourself so we can correspond with you and try to make sure everybody on the list is a bona fide member.

I will enable your account after I receive your response.

(Minors - people not of legal age - are not allowed to use the site. So if you are under age, please do not continue.)

I\'ll be happy to approve your account. Your username and password (active after we correspond) are below.

Randy Fay
Warmshowers.org webmaster

AFTER your account has been authorized (after I receive your response and
activate your account), your username and password details will be:

username: [user:name]


AFTER your account is approved, you may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you can always change your password and other account details at  [user:edit-url]

Also: After you log in, please go to "my account" and the "map my location" tab and make sure your location shows correctly on the map. There are instructions on that page that will help you set it.

Thanks for signing up!

-Randy Fay and the registration team
Warmshowers.org

--
Administrator use only:
[user:edit-url]',
  'user_mail_register_pending_approval_subject' => 'Account info for  [user:name] at WarmShowers.org-You must reply and say something about yourself',
  'user_mail_status_activated_body' => 'Hola [user:name]

Su cuenta en www.warmshowers.org ha sido activado y que ahora tienen acceso completo al sitio web. Puede entrar en cualquier momento con el nombre de usuario y contraseña creada durante el registro.

Si usted tiene problemas o preguntas que nos envíe un correo electrónico y haremos nuestro mejor esfuerzo para ayudar.',
  'user_mail_status_activated_subject' => '[Warmshowers.org] Su cuenta ha sido aprobada',
  'user_mail_status_blocked_body' => 'Hola [user:name]:

Tu cuenta en www.warmshowers.org ha sido bloqueada.

Si necesitas mayor información, por favor responde a este e-mail.

Gracias.
Randy Fay.
Webmaster de Warmshowers.org

Acceso admin:
[user:edit-url]
',
  'user_mail_status_blocked_subject' => 'Warmshowers.org: La cuenta [user:name] ha sido bloqueada',
  'user_mail_status_deleted_body' => 'Hola [user:name] -

Su cuenta en es.warmshowers.org se ha suprimido.

Si usted tiene alguna pregunta, por favor, responda a este mensaje.

Gracias,
Randy Fay
Webmaster Warmshowers.org',
  'user_mail_status_deleted_subject' => 'Warmshowers.org: [user:name] cuenta cancelada',
  'user_picture_guidelines' => 'Queremos una foto de  <i>ti</i>, tal vez montando en bici o a tu casa. Este is opcional, pero divertido, también. Fotos al azar  aparecen en la página principal. ',
  'user_registration_help' => '<h3>¡Bienvenido! Por favor, lee esto antes de seguir adelante</h3>
¡Nos complace tu registro en WarmShowers! Te pedimos un minuto para que rellenes toda tu información. Luego te enviaremos un e-mail con un enlace de validación. Por supuesto que con la cantidad de filtros anti-spam es posible que nuestro correo no te aparezca si no nos añades a tu lista de correos autorizados. Es importante que verifiques que nuestro correo no se ha filtrado.


<ol><li>
Así que, antes de continuar con tu registro, añade por favor wsl@warmshowers.org a tu lista autorizada. <a href="https://www.warmshowers.org/spam_filters" target="blank">Aquí</a> tienes instrucciones de cómo realizarlo en varios proveedores de correo electrónico. Si no lo haces, es posible que no recibas el correo electrónico de validación que contiene tu contraseña y no podrás registrarte.
<li>Por favor, echa un vistazo a <a href="/the_rules" target="_blank">"la reglas"</a> y recuerda revisarlas cuando tengas alguna duda.</li>
<li>Recuerda que al registrarte aceptas que estás dispuesto a ofrecer tu hospitalidad, al menos en el futuro. Esta es una página de hospitalidad recíproca. (Si estás viajando o no puedes ofrecer por el momento hospitalidad marca la casilla "No disponible actualmente" en tu información personal, y podrás desmarcarla en el futuro.)</li>
<li>Los menores de edad no están autorizados a registrarse en esta web, así que, por favor, no continúes si eres menor de edad.</li>
<li>Recuerda que tenemos una sección de <a href="/faq" target="_blank">Preguntas frecuentes (FAQ)</a>, donde puedes resolver algunos de tus problemas. Consúltalas también si no puedes acceder a tu cuenta.</li>
</ol>
<br/><br/>
¡Esto es todo! Te invitamos pues a continuar con tu registro más abajo.
<hr/>
',
  'wsuser_become_available_notification_body' => 'Hi [user:fullname] - <br/><br/>Your Warmshowers.org account, [user], has been automatically updated to show you as "available to host" because the date that you specified you would become available after has passed. If you are still unavailable to host please log into Warmshowers.org and update your account. You can log in by visiting the site or just <a href=\'[user:login_url]\'>clicking this link</a>. Thanks!<br/><br/>-Randy Fay<br/>Warmshowers.org Webmaster<br/>---<br/><a href=\'[user:url]\'>Profile link</a><br/><a href=\'[account-edit-url]\'>Edit profile link</a><br/><a href=\'[user:login_url]\'>Direct login link</a><br/>',
  'wsuser_become_available_notification_subject' => 'You are now listed as available to host on Warmshowers.org ([user:fullname])',
  'wsuser_notavailable_reminder_body' => 'Hi [user:fullname], <br/><br/>Your warmshowers.org account, [user], is marked as unavailable to host. Could you please log in and set the date that you expect to return? If you don\'t know when or if you\'ll be able to host, just put in a date like 2025-01-01. You can change it at any time. You can log in with your username and password at <a href=\'https://es.warmshowers.org\'>es.warmshowers.org</a> or <a href=\'[user:login_url]\'>click this link to log in automatically</a><br/><br/>-Randy Fay<br/>Warmshowers.org Webmaster<br/>---<br/><a href=\'[user:url]\'>Profile link</a><br/><a href=\'[account-edit-url]\'>Edit profile link</a><br/><a href=\'[user:login_url]\'>Direct login link</a><br/>',
  'wsuser_notavailable_reminder_subject' => 'Your Warmshowers account is marked unavailable. Please update your return date. ([user:fullname])',
);
  $realm_variables['language']['fa'] = array(
  'contact_form_information' => '<p>خوشوقت می شویم از اینکه  از اینجا با ایمیل با ما تماس بگیرید..</p>

<p> اگر برای استفاده از سایت نیاز به راهنمایی دارید، لطفا ابتدا <a href="/faq"> صفحه سوالات رایج را بخوانید </a> ، و سپس اگر پاسخ سوال خود را نیافتید  به  بخش <a href="http://www.warmshowers.org/forums/website-help-and-support"> راهنمایی انجمن </a>  بروید و سوال خود را در آنجا ارسال دارید . با اینکار نه تنها دیگران به شما کمک می کنند بلکه پاسخها در آیند ه مورد استفاده دیگر اعضا نیز خواهد بود. </p>


<p>چنانچه سوال خود را به زبان انگلیسی بیان کنید پاسخ دادن به آن <b> بسیار بسیار</b>  آسانتر خواهد بود .</p>',
  'inactive_user_block_notify_text' => 'Hello %username,

  This automatic message is to notify you that your account on %sitename has been automatically disabled due to no activity for more than %period.

  Please visit us at %siteurl to have your account re-enabled.

Sincerely,
  %sitename team',
  'inactive_user_block_warn_text' => 'Hello %username,

  We haven\'t seen you at %sitename since %lastaccess, and we miss you!  This automatic message is to warn you that your account will be disabled in %period unless you come back and visit us before that time.

  Please visit us at %siteurl.

Sincerely,
  %sitename team',
  'inactive_user_delete_notify_text' => 'Hello %username,

  This automatic message is to notify you that your Warmshowers.org account has been automatically removed because you didn\'t log in for more than %period.

  Please visit us at %siteurl if you would like to create a new account.

Sincerely,
-Randy Fay
Warmshowers.org Webmaster',
  'inactive_user_delete_warn_text' => 'Hello %username,

Your account at Warmshowers.org will soon be removed (in %period) unless you log in to let us know you still want it. Please log into the site at http://www.warmshowers.org and update your account.  If you don\'t know your username or password that\'s OK - you can log in with your email address. If you don\'t know your password, please visit http://www.warmshowers.org/user/password to get a one-time login link emailed to you.

You haven\'t logged in %lastaccess.   This automatic message is to warn you that your account will be completely removed in %period unless you log into the site before that time.

  Please visit us at %siteurl.

Sincerely,
-Randy Fay
Warmshowers.org webmaster',
  'inactive_user_notify_text' => 'سلام %username,


شما از تاریخ %lastaccess تا حالا وارد سایت نشده اید، و جایتان خالی است. ما از همه اعضا خواسته ایم که حد اقل سالی یکبار وارد حسابشان شوند و اطلاعات سخصی شان را چک کنند اگر لازم بود آنها را بروز کنند، پس باعث خرسندی است اگر حسابتان در وارم شاورز را چک کنید و اگر لازم بود آنرا بروز کنید. یا حد اقل فقط  *وارد سیستم سایت شوید*  تا سیستم از دایر بودن این آدرس ایمیلتان اطمینان حاصل کند.
لطفاً  هرچه سریعتر  حسابتان را در %siteurl بروز کنید . اگر در طی یک سال تقویمی  حتی یکبار وارد سایت نشوید ، حسابتان حذف خواهد شد .(ولی اول چند تا هشدار دریافت خواهید کرد)
اگر کلمه عبورتان را فراموش کرده اید، کافی است به http://www.warmshowers.org/user/password بروید و آدرس ایمیلتان را (%useremail) بگذارید. پس از آن یک لینک به آدرس شما ارسال می شود که با کلیک کردن آن  می توانید تنها یکبار به حسابتان وارد شوید تا نتیجتا  بتوانید کلمه عبور خود را تغییر دهید.

ارادتمند
-Randy Fay
رئیس وبسایت وارم شاورز

------
Admin Information
Search: http://www.warmshowers.org/search/wsuser/%useremail
Username: %username
Email: %useremail
',
  'user_mail_password_reset_body' => 'سلام[user:name]
شما ( یا یک کسی) برای اجازه یکبار ورود به حساب شما در  warmshowers.org  درخواست کرده است.اگر شما درخواست کننده نبوده اید، کافی است فقط این پیغام را ندیده بگیرید.
لطفا ً  برای بازنویسی کلمه رمزتان <a href="[user:one-time-login-url]">اینجا را کلیک کنید </a>، تا بتوانید  کلمه رمزتان را تغییر دهید.

[user:one-time-login-url]

بعد از ورود به سایت ، شما به<a href="[user:edit-url]"> صفحه ویرایش اطلاعات شخصی </a>  منتقل می شوید. تا بتوانید کلمه رمز خود را تغییر دهید.
با تشکر،
-Randy Fay and the registration team
WarmShowers.org


رفع اشکال

اگر پیغام  "متاسفم – شما دسترسی به آن صفحه را ندارید (خطای 403)" را دریافت کردید، به احتمال خیلی زیاد به معنی اینست که شما کوکی ها را روی مرورگر خود خاموش کرده اید. برای تنظیم صحیح کوکی ها لطفا ً ا<a href="http://www.warmshowers.org/content/i-try-log-it-still-says-im-not-logged">ین مطلب را در بخش "سوالات رایج" </a>ببینید.

اگر نتوانستید این مشکل را حل کنید، لطفا ً از ما کمک بخواهید و  این  ایمیل را  Reply  کنید.

[Warmshowers.org] عضویت انجام شد .
خوشامدید[user:name] !

حساب شما در www.warmshowers.or  فعال شده است .

شما می توانید با کلمه کاربری و کلمه رمزی که دارید وارد سایت شوید، و یا می توانید همین حالا با کلیک کردن<a href="[user:one-time-login-url]"> اینجا</a>  کلمه رمزتان را تغییر دهید.

این لینک تنها برای یکبار ورود قابل استفاده است
بعد از ورود به سایت شما به <a href="[user:edit-url]">صفحه ویرایش</a>  مشخصات منتقل می شوید و می توانید کلمه رمزتان را تغییر دهید.

سئوالی  هست؟  کافی است  از طریق همین ایمیل  ( و با Reply  کردن ) آنرا بپرسید.

با تشکر،
-Randy Fay
Warmshowers.org webmaster

--
فقط برای استفاده مدیریت
!uri
',
  'user_mail_password_reset_subject' => '[Warmshowers.org] لینک یکبار اجازه یکبار ورود برای [user:name]',
  'user_mail_register_admin_created_body' => '[user:name],

A site administrator at !site has created an account for you. You may now log in to !login_uri using the following username and password:

username: [user:name]


You may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you will be redirected to [user:edit-url] so you can change your password.


--  !site team',
  'user_mail_register_admin_created_subject' => 'An administrator created an account for you at  !site',
  'user_mail_register_no_approval_required_body' => '[user:name],

از عضویت شما در
 Warmshowers.org
 متشکریم و بهتون خوشامد میگوییم.
لطفا برای تایید صحت آدرس ایمیل تان <a href="[user:one-time-login-url]">اینجا</a> را کلیک کنید تا حساب شما فعال شود.
اگر کلیک کردن آن ممکن نشد ، لطفا لینک زیر  را در نوار آدرس مرورگر خود کپی کنید:
[user:one-time-login-url]
بعد از تایید شدن از طریق لینک فوق ، شما خواهید توانست با استفاده از کلمه کاربری و کلمه رمز زیر به
http://fa.warmshowers.org/user
وارد شوید.

کلمه کاربری: [user:name]
کلمه رمز: کلمه رمزی که در فرم وارد کردید
هر سوالی دارید، می توانید با
 "reply"
 کردن این ایمیل از ما بپرسید و ما حداکثر سعیمان را برای کمک به شما می کنیم.
با تشکر،
--!site team
-- فقط برای استفاده مدیریت
مدیریت(ویرایش): <a href="[user:edit-url]">[user:edit-url]</a>',
  'user_mail_register_no_approval_required_subject' => '[Warmshowers.org] تایید عضویت شما در Warmshowers.org',
  'user_mail_register_pending_approval_body' => 'Welcome [user:name],

*** You must reply to this email to get access to the site! ***
*** Please reply saying something about yourself! ***
*** You must leave the message\'s subject and the rest of this message as they are. ***

( And when you reply - we need you to say "hi". You\'re not replying to a machine but to Randy and the rest of the team! )

*** You won\'t be able to log in until we enable your account
*** which is usually at least several hours after you send your reply.

Thank you for your registration at www.warmshowers.org.

Thank you for applying for an account, and for your hospitality to touring cyclists.

You need to acknowledge that you\'ve received this message by replying to it and saying something about yourself so we can correspond with you and try to make sure everybody on the list is a bona fide member.

I will enable your account after I receive your response.

(Minors - people not of legal age - are not allowed to use the site. So if you are under age, please do not continue.)

I\'ll be happy to approve your account. Your username and password (active after we correspond) are below.

Randy Fay
Warmshowers.org webmaster

AFTER your account has been authorized (after I receive your response and
activate your account), your username and password details will be:

username: [user:name]


AFTER your account is approved, you may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you can always change your password and other account details at  [user:edit-url]

Also: After you log in, please go to "my account" and the "map my location" tab and make sure your location shows correctly on the map. There are instructions on that page that will help you set it.

Thanks for signing up!

-Randy Fay and the registration team
Warmshowers.org

--
Administrator use only:
[user:edit-url]',
  'user_mail_register_pending_approval_subject' => 'Account info for  [user:name] at WarmShowers.org-You must reply and say something about yourself',
  'user_mail_status_activated_body' => 'خوشامدید[user:name] !

حساب شما در www.warmshowers.or  فعال شده است .

شما می توانید با کلمه کاربری و کلمه رمزی که دارید وارد سایت شوید، و یا می توانید همین حالا با کلیک کردن<a href="[user:one-time-login-url]"> اینجا</a>  کلمه رمزتان را تغییر دهید.

این لینک تنها برای یکبار ورود قابل استفاده است
بعد از ورود به سایت شما به <a href="[user:edit-url]">صفحه ویرایش</a>  مشخصات منتقل می شوید و می توانید کلمه رمزتان را تغییر دهید.

سئوالی  هست؟  کافی است  از طریق همین ایمیل  ( و با Reply  کردن ) آنرا بپرسید.

با تشکر،
-Randy Fay
Warmshowers.org webmaster

--
فقط برای استفاده مدیریت
!uri
',
  'user_mail_status_activated_subject' => ' [Warmshowers.org] عضویت انجام شد .',
  'user_mail_status_blocked_body' => '[Warmshowers.org] حساب شما مسدود شد.
سلام [user:name]
حساب شما  در www.warmshowers.org  مسدود شد.
هر سئوالی دارید لطفا با پاسخ دادن به همین ایمیل بپرسید

با تشکر،
-Randy Fay
Warmshowers.org webmaster


--
فقط برای استفاده مدیریت
[user:edit-url]
',
  'user_mail_status_blocked_subject' => '[Warmshowers.org] حساب شما مسدود شد.',
  'user_mail_status_deleted_body' => 'حساب شما  در www.warmshowers.org  حذف شده اس ، احتمال زیاد بخاطر درخواست شما و یا به علت نامعتبر بودن  آن.
هر سئوالی دارید لطفا با پاسخ دادن به همین ایمیل بپرسید


با تشکر،
-Randy Fay
Warmshowers.org ',
  'user_mail_status_deleted_subject' => '[Warmshowers.org] حساب [user:name] حذف گردید.',
  'user_picture_guidelines' => 'ما دوستداریم عکسی از<i> شما</i> داشته باشیم ، احتمالا در حال دوچرخه سواری  و یا در خانه تان.  البته این کاملا ً اختیاری است ،  ولی میتواند سرگرم کننده باشد. زیرا ما در صفحه اول سایت از بین عکسهای اعضا ، عکسهایی را  بطور اتفاقی اتخاب کرده و به نمایش می گذاریم. اگر یک عکس جدید بارگذاری کردید ولی همچنان  عکس قدیمی به نمایش درآمد، یک  shift-refresh   روی مرورگر خود انجام دهید.( کلید  SHIFT  را فشرده نهداشته و همزمان  نمایه REFRESH   را کلیک کنید.) </i>',
  'user_registration_help' => '<h3>خوش آمدید! لطفا ً قبل از ادامه ، مطلب زیر را بخوانید. </h3>
ما خوشحالیم که شما میخواهید در وارم شاورز عضو شوید! تا دقایقی دیگر ، شما اطلاعات خود را وارد خواهید کرد، و سپس ما یک ایمیل  تاییدی به شما خواهیم فرستاد –  شما باید یک لینک در این ایمیل را کلیک کنید. ولی البته این روزها با وجود میلیونها فیلتر مختلف ، ممکن است ایمیل ما یه صندوق ورودی  ایمیل شما نرود، و در صورتی که این ایمیل به شما نرسد ، نم نویسی و عضویت شما هیچوقت کامل  نمی شود.   مگر اینکه قبلا  آدرس  ایمیل ما را به  عنوان ایمیل  قابل اعتماد ( غیر مزاحم) در لیست سفید وارد کرده باشیدد و یا در دفترچه آدرسها، در ایمیل خود، ثبت کرده باشید.بنابر این...

<ol><li>
لطفا همین الان قبل از ارسال این فرم ، آدرس wsl@warmshowers.org را به لیست سفید خود ( یعنی آدرسهای مطمئنی که بعنوان غیر اسپم به اکانت ایمیل خود معرفی می کنید) اضافه کنید. و یا یک ایمیل خالی  برای ما ارسال کنید . دقت کنید که در محل موضوع  یا تیتر آن ایمیل عبارت test  را تایپ کنید.

اگر اینکار را نکنید ، ممکن است که ایمیل تصدیق نهایی که شامل کلمه رمز شما برای ورود به سایت است به دست شما نرسد و بدین ترتیب روال نام نویسی شما ناتمام و نا معتبر خواهد شد.

 دستتان نرسد و  عضویت شما انجام نمی شود. .</li>
<li>لطفا صفحه <a href="/the_rules" target="_blank">قوانین </a>  را مطالعه کنید و هر وقت مردد بودید مجددا آنرا بخوانید. .</li>
<li>هنگامی که برای عضویت نام نویسی می کنید ، یادتان باشد که  شما تصمیم گرفته اید حد اقل  زمانی  میزبان  دوچرخه سواران باشید. این یک سایت میزبانی  متقابل است .  ( اگر اکنون در سفر هستید  یا به هر دلیلی  نمی توانید میزبان باشید ، گزینه "فعلا در دسترس نیستم " را علامت بزنید و بعدا وقتی آمادگی پذیرش مهمان  داشتید این علامت را بردارد.) </li>
<li>افرادی که به سن قانونی نرسیده اند اجازه استفاده از این سایت را ندارند . پس اگر چنین است  لطفا در همین جا پایان دهید. </li>
<li>یاد آوری می کنم که ما یک صفحه با عنوان<a href="/faq" target="_blank"> سئوالات رایج </a> داریم که  اگر ابهامی دارید ممکن است  بتواند آنرا حل کند. و به یاد داشته باشید که  اگر در ورود به سایت مشکلی داشتید ، ابتدا آنرا چک کنید. </li>

<li>آیا برایتان مهم است که چه کسی اطلاعات شما در اینجا  را می بیند؟  این وبسایت طوری طراحی شده است که اطلاعات شما برای جهان خارج از این وبسایت قابل مشاهده نیستک تنها اعضای این انجمن بعد از ورود به سایت قادر خواهد بود که اطلاعات دیگر اعضا را مشاهده کنند. در مورد آدرس ایمیل شما، حتی برای دیگر اعضا قابل مشاهده نیست. </li>
<li>امنیت شخصی شما نیز بسیار مهم است. لطفاً صفحه<a href="/personal-security"> امنیت شخصی </a>  که در باره این موضوع است را مطالعه کنید. </li>
<li>سیاست ما در مورد  <a target="_blank" href="/privacy">احترام به حریم خصوصی </a>به شما یاد آور می شود که مشخصات شما تنها برای اعضا قابل مشاهده باشند. و تنها اعضا بتوانند با شما تماس بگیرند. </li>
<li>با نام نویسی  برای عضو  شدن ، شما با   <a target="_blank" href="/terms">شرایط استفاده </a>  موافقت کرده اید و آن تاکید دارد بر اینکه <strong>تنها شما مسئول مراوده های خودتان با دیگر اعظا هستید. </strong></li>
</ol>
<br/><br/>
خیلی خوب ، همش همین بود!
لطفا روال را با پر کردن فرم نام نویسی ذیل ادامه دهید.
<hr/>
',
  'wsuser_become_available_notification_body' => 'سلام [user:fullname], <br/><br/>
 حساب شما در وارمشاورز
  [user]
 بطور اتوماتیک بروز شده تا شمارا بعنوان "در دسترس برای مهمان پذیری" نمایش دهد زیرا بر مبنای زمانی که شما تنظیم کرده بودید اکنون در دسترس هستید.
اگر همچنان امکان پذیرا شدن مهمان را ندارید لطفا به سایت وارد شده و صفحه شناسنامه یا همان پروفایل خود را بروز کنید.
 شما میتوانید با نام کاربری و کلمه عبور خود از طریق زیر وارد شوید.

<a href=\'https://fa.warmshowers.org\'>fa.warmshowers.org</a> or <a href=\'[user:login_url]\'>برای ورود اتوماتیک این لینک را کلیک کنید</a><br/><br/>-Randy Fay<br/>Warmshowers.org Webmaster<br/>---<br/><a href=\'[user:url]\'>لینک پروفایل (صفحه شناسنامه)</a><br/><a href=\'[account-edit-url]\'>لینک ویرایش پروفایل</a><br/><a href=\'[user:login_url]\'>لینک مستقیم ورود</a><br/>
',
  'wsuser_become_available_notification_subject' => 'شما اکنون در لیست میزبانان فعال در وارم شاورز هستید.([user:fullname])',
  'wsuser_notavailable_reminder_body' => 'حساب وارمشاورز شما بعنوان "فعلا در دسترس نیستم" علامت خورده است. لطفا تاریخ برگشت خود را به روز کنید. ([user:fullname])
<a href=\'https://fa.warmshowers.org\'>fa.warmshowers.org</a> or <a href=\'[user:login_url]\'>برای ورود اتوماتیک این لینک را کلیک کنید</a><br/><br/>-Randy Fay<br/>Warmshowers.org Webmaster<br/>---<br/><a href=\'[user:url]\'>Profile link</a><br/><a href=\'[account-edit-url]\'>Edit profile link</a><br/><a href=\'[user:login_url]\'>Direct login link</a><br/>

',
  'wsuser_notavailable_reminder_subject' => 'حساب وارم شاورز شما بعنوان "فعلا در دسترس نیستم" علامت خورده است. لطفا تاریخ برگشت خود را به روز کنید. ([user:fullname])',
);
  $realm_variables['language']['fr'] = array(
  'anonymous' => 'Anonymous',
  'comment_notify_default_mailtext' => 'Bonjour [comment-subscribed:author:fullname],

[comment:author:fullname] a rédigé un commentaire sur : "[comment:node:title]":

----
[comment:title]
[comment:body]
----

Pour visualiser le commentaire et y répondre, <a href="[comment:url]">cliquez ici</a>. Ne répondez pas à cet e-mail, car la réponse irait au webmaster et non pas à l\'auteur du commentaire.

Pour arrêter de recevoir un e-mail à chaque fois que quelqu\'un commente votre post, <a href="[comment-subscribed:unsubscribe-url]">cliquez ici</a>

- L\'équipe Warmshowers.org',
  'contact_form_information' => '<p>Vous pouvez bien entendu nous contacter par mail.</p>

<p>Si vous avez besoin d\'aide pour utiliser le site, commencez par lire la <a href="/faq">Foire Aux Questions</a>, et lisez le <a href="http://www.warmshowers.org/forums/website-help-and-support">forum d\'aide</a> et postez-y vos propores problèmes s\'ils ne sont pas encore résolus ainsi d\'autres membres pourront vous aider et leurs réponses pourront aider d\'autres membres.</p>

<p>If you speak English it will help the admins a *lot* if you can ask your question in English. Thanks! </p>',
  'faq_description' => '',
  'faq_title' => 'Foire Aux Questions',
  'inactive_user_block_notify_text' => 'Hello %username,

  This automatic message is to notify you that your account on %sitename has been automatically disabled due to no activity for more than %period.

  Please visit us at %siteurl to have your account re-enabled.

Sincerely,
  %sitename team',
  'inactive_user_block_warn_text' => 'Hello %username,

  We haven\'t seen you at %sitename since %lastaccess, and we miss you!  This automatic message is to warn you that your account will be disabled in %period unless you come back and visit us before that time.

  Please visit us at %siteurl.

Sincerely,
  %sitename team',
  'inactive_user_delete_notify_text' => 'Hello %username,

  This automatic message is to notify you that your Warmshowers.org account has been automatically removed because you didn\'t log in for more than %period.

  Please visit us at %siteurl if you would like to create a new account.

Sincerely,
-Randy Fay
Warmshowers.org Webmaster',
  'inactive_user_delete_warn_text' => 'Bonjour %username,

Votre compte sur Warmshowers.org sera bientôt supprimé (dans %period) à moins que vous vous connectiez d’ici là ce qui signifierai pour nous que vous en voulez encore. Connectez vous donc sur http://warmshowers.org et mettez à jour votre compte. Si vous ne souvenez plus de votre login, pas de soucis, vous pouvez vous connecter avec cette adresse mail, si vous ne vous souvenez plus de votre mot de passe, visitez http://www.warmshowers.org/user/password et vous recevrez un mail avec un lien de connexion unique.

Vous ne vous êtes plus connecté depuis %lastaccess.   Ce message automatique vous est envoyé pour vous alerter du fait que votre compte sera totalement supprimé dans %period à moins que vous ne vous connectiez d’ici là.

  rendez nous visite sur %siteurl.

Cordialement,
-Randy Fay
Warmshowers.org webmaster',
  'inactive_user_notify_text' => 'Bonjour %username,

Vous ne vous êtes pas connecté sur Warmshowers.org depuis %lastaccess, et vous nous manquez! Nous demandons à nos utilisateurs de se connecter au moins une fois par an pour être sûr que leurs informations sont à jour, c’est pourquoi nous vous demandons aujourd’hui de vous connecter et de mettre à jour votre profil. Si vous n’avez rien à modifier, cela nous permettra au moins d’être sûr que vous utilisez toujours cette adresse mail.

S’il vous plaît, connectez vous rapidement à https://fr.warmshowers.org Si vous ne le faites pas pendant une année entière, votre compte pourrait tout simplement être effacé (mais vous recevrez bien entendu des quelques mails d’alerte avant cela).

Si vous avez oublié votre mot de passe, allez juste sur http://www.warmshowers.org/user/password et insérez votre adresse mail (%useremail) et vous recevrez un mail avec un lien de connexion unique (valable une fois).

Cordialement,,
-Randy Fay
Warmshowers.org webmaster

------
Admin Information
Search: http://www.warmshowers.org/search/wsuser/%useremail
Username: %username
Email: %useremail
',
  'node_notify_default_mailtext' => 'Bonjour [comment:node:author:fullname],

Vosu avez reçu un commentaire sur: "[comment:node:title]":

----
[comment:title]
[comment:body]
----

Pour visualiser le commentaire et y répondre, <a href="[comment:url]">cliquez ici</a>. Ne répondez pas à cet e-mail, car la réponse irait au webmaster et non pas à l\'auteur du commentaire.

Vous recevrez un e-mail comme celui-ci à chaque fois que quelqu\'un répond à votre post.

- L\'équipe Warmshowers.org
',
  'site_footer' => 'Hébergement entre cyclo-randonneurs dans le monde entier<br/>
<a href="/">Accueil</a>  <a href="/map">Carte</a>',
  'site_mission' => 'Membres de <a href="http://fr.warmshowers.org">warmshowers.org</a>: hébergement entre cyclo-randonneurs',
  'site_name' => 'Warmshowers.org',
  'site_slogan' => 'Hébergement entre cyclo-randonneurs',
  'user_mail_password_reset_body' => 'Bonjour [user:name]:

Vous (ou un tiers) avez fait la demande de réinitialisation du mot de passe pour accéder à votre profil warmshowers.org. Si vous n\'êtes pas à l\'origine de cette demande, ignorez simplement ce message.

Vous pouvez maintenant vous identifier <a href="[user:one-time-login-url]">en cliquant ici</a>.

[user:one-time-login-url]

Ceci est un lien à usage unique. Sa validité est de 24h, et il ne se passe rien si vous ne l´utilisez pas !

Une fois identifié, vous serez redirigé vers <a href="[user:edit-url]">à votre profil de compte</a> afin de pouvoir changer votre mot de passe.

Merci.

-Randy Fay et l\'équipe de WarmShowers.org


-------------------
En cas de problème :

  Si vous avez un message du type : "Désolé, vous n´avez pas accès à cette page (code erreur 403)", c´est probablement que les cookies sont désactivés sur votre navigateur.



Vu dans la FAQ :

Je clique sur le lien à usage unique pour pouvoir changer mon mot de passe, et j\'obtiens un message "Accès refusé"

Le problème vient généralement du fait que les cookies sont désactivés sur votre navigateur ou qu´il n\'accepte pas les cookies. Pour utiliser le site, vous devez autoriser les cookies. Ceux-ci ne constituent pas une menace pour votre ordinateur. La plupart des sites internet en ont besoin pour fonctionner correctement. Pour autoriser les cookies, vous pouvez :

    * Pour Internet Explorer, cliquez sur Outils->Options Internet->Privacy et sélectionnez toute option inférieure à "block all cookies” ou...
    * Cliquez sur "sites" dans la même fenêtre de dialogue et autorisez les cookies pour www.warmshowers.org ou...
    * Dans certains cas, IE ne se comporte pas comme prévu et il faut alors effacer tous les cookies (Outils ->Options Internet->General->Delete Cookies)
    * En profiter pour essayer l\'excellent navigateur libre Firefox. Si vous ne pouvez pas installer Firefox parce que vous n\'avez pas les droits suffisants sur votre ordinateur, essayez Firefox Portable Edition, qui ne nécessite aucune installation (quand il est en voyage, Randy l\'emmène sur une clé USB pour éviter les problèmes en Amérique Latine).

    * Pour Internet Explorer 7 la plupart des problèmes sont résolus en cliquant sur Outils-> Options Internet et après avoir cliqué sur l´onglet "Advanced”, cliquez sur le bouton "Reset".
    * Assurez-vous d´avoir vidé la mémoire cache: pour IE6 Outils->Options Internet->Delete Files. Dans  Firefox, cliquez sur Outils->Effacer mes traces; cochez "cache" et "sessions identifiées / authentifiées " et cliquez sur "effacer mes traces maintenant".
    * Tout programme qui bloque les cookies peut aussi causer ce problème. Norton peut ainsi, selon les réglages, bloquer les cookies pour tous les programmes. Essayez de désactiver temporairement votre firewall et votre antivirus, pour voir s\'ils sont la cause du problème.
    *Si le problème persiste, demandez de l\'aide. Répondez à cet e-mail et décrivez votre problème.',
  'user_mail_password_reset_subject' => '[Warmshowers.org] E-mail de réinitialisation du mot de passe ([user:name])',
  'user_mail_register_admin_created_body' => '[user:name],

A site administrator at !site has created an account for you. You may now log in to !login_uri using the following username and password:

username: [user:name]

You may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you will be redirected to [user:edit-url] so you can change your password.


--  !site team',
  'user_mail_register_admin_created_subject' => 'An administrator created an account for you at  !site',
  'user_mail_register_no_approval_required_body' => 'Bonjour [user:name]:

Merci de vous être inscrit à Warmshowers.org

Veuillez <a href="[user:one-time-login-url]">cliquez ici</a> pour valider votre adresse e-mail et votre inscription.

[user:one-time-login-url]

Après avoir validé votre adresse en utilisant le lien ci-dessus, vous pourrez vous identifier sur le site avec l\'identifiant et le mot de passe suivants :

identifiant: [user:name]

Si vous avez des questions, veuillez répondre à cet e-mail. Nous ferons de notre mieux pour vous aider.

Merci

Warmshowers.org l\'administration
----
Admin (edit): <a href="[user:edit-url]">[user:edit-url]</a>',
  'user_mail_register_no_approval_required_subject' => 'Validez votre inscription sur Warmshowers.org ([user:name])',
  'user_mail_register_pending_approval_body' => 'Welcome [user:name],

*** You must reply to this email to get access to the site! ***
*** Please reply saying something about yourself! ***
*** You must leave the message\'s subject and the rest of this message as they are. ***

( And when you reply - we need you to say "hi". You\'re not replying to a machine but to Randy and the rest of the team! )

*** You won\'t be able to log in until we enable your account
*** which is usually at least several hours after you send your reply.

Thank you for your registration at www.warmshowers.org.

Thank you for applying for an account, and for your hospitality to touring cyclists.

You need to acknowledge that you\'ve received this message by replying to it and saying something about yourself so we can correspond with you and try to make sure everybody on the list is a bona fide member.

I will enable your account after I receive your response.

(Minors - people not of legal age - are not allowed to use the site. So if you are under age, please do not continue.)

I\'ll be happy to approve your account. Your username and password (active after we correspond) are below.

Randy Fay
Warmshowers.org webmaster

AFTER your account has been authorized (after I receive your response and
activate your account), your username and password details will be:

username: [user:name]

AFTER your account is approved, you may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you can always change your password and other account details at  [user:edit-url]

Also: After you log in, please go to "my account" and the "map my location" tab and make sure your location shows correctly on the map. There are instructions on that page that will help you set it.

Thanks for signing up!

-Randy Fay and the registration team
Warmshowers.org

--
Administrator use only:
[user:edit-url]',
  'user_mail_register_pending_approval_subject' => 'Account info for  [user:name] at WarmShowers.org-You must reply and say something about yourself',
  'user_mail_status_activated_body' => 'Bonjour [user:name]:

Votre compte à www.warmshowers.org a été activé et que vous avez maintenant un accès complet au site. Vous pouvez vous connecter à tout moment avec le nom d\'utilisateur et mot de passe créés lors de votre inscription.

Si vous avez des problèmes ou des questions envoyez nous un email et nous ferons de notre mieux pour aider.',
  'user_mail_status_activated_subject' => '[Warmshowers.org] Votre compte a été approuvé',
  'user_mail_status_blocked_body' => 'Hola [user:name]:

Tu cuenta en www.warmshowers.org ha sido bloqueada.

Si necesitas mayor información, por favor responde a este e-mail.

Gracias.
Randy Fay.
Webmaster de Warmshowers.org

Acceso admin:
[user:edit-url]
',
  'user_mail_status_blocked_subject' => 'Warmshowers.org: La cuenta [user:name] ha sido bloqueada',
  'user_mail_status_deleted_body' => 'Bonjour [user:name]

Votre compte sur fr.warmshowers.org a été supprimé.

Si vous avez des questions, s\'il vous plaît répondez à ce message.

Merci,
Randy Fay
Warmshowers.org webmaster',
  'user_mail_status_deleted_subject' => 'Warmshowers.org: [user:name] compte supprimé',
  'user_picture_guidelines' => 'Instructions pour les photos:<br>
Nous aimerions avoir une photo de <i>vous</i>, sur votre vélo ou chez vous. Rien d\'obligatoire, bien sûr, mais cela peut être amusant. Sur la page d\'accueil, vous pouvez voir des photos prises au hasard. <i>Si vous téléchargez une nouvelle photo, et qu\'elle n\'apparait pas immédiatement, il faut rafraîchir votre affichage. (Maintenir la touche Maj enfoncée, et cliquer sur le bouton "actualiser la page courante" de votre navigateur)</i>
',
  'user_registration_help' => '<h3>Bienvenue! Veuillez lire ceci avant de poursuivre</h3>
Nous sommes heureux de vous accueillir à Warm Showers ! Nous allons vous demander quelques renseignements, et nous vous enverrons un e-mail de validation, avec un lien sur lequel vous devrez cliquer pour valider définitivement votre inscription. Cependant avec les filtres destinés à éliminer les spams (e-mails non sollicités), vous risquez fort de ne pas recevoir cet e-mail si vous ne nous avez pas inscrit dans votre "liste des contacts autorisés". Dans ce cas, votre inscription ne sera jamais validée. Alors ...

<ol><li>
Avant d\'envoyer ce formulaire, veuillez ajouter wsl@warmshowers.org à votre "liste des contacts autorisés". Si vous ne le faites pas vous risquez de ne pas recevoir l´e-mail de validation avec votre mot de passe, et votre inscription ne sera pas validée.</li>
<li>Lisez <a href="/the_rules" target="_blank">les règles</a> et pensez à les consulter chaque fois que vous avez un doute.</li>
<li>En vous inscrivant, vous signifiez votre souhait d´héberger d´autres membres dès que vous le pourrez. Ce site propose l´hébergement sur la base de la réciprocité. Si vous êtes vous-même en voyage ou si pour d´autres raisons vous ne pouvez temporairement accueillir de cyclo-randonneur, cochez la case "Non disponible actuellement" dans votre profil. Vous la décocherez quand vous serez à nouveau disponible pour héberger des cyclo-randonneurs.</li>
<li>Les personnes qui n\'ont pas atteint l\'âge légal de la majorité ne sont pas autorisées à s\'inscrire. Veuillez donc quitter ce site si vous êtes mineur.</li>
<li>Pensez à consulter notre <a href="/faq" target="_blank">Foires Aux Questions</a> en cas de problème, en particulier si vous avez du mal à vous identifier sur le site.</li>
<li>Nos <a target="_blank" href="/privacy">règles de confidentialité</a> stipulent que votre profil n’est visible que des membres et que seuls les membres peuvent vous contacter.</li>
<li>En vous inscrivant, vous acceptez nos <a target="_blank" href="/terms">règles d’utilisation</a> qui précisent bien  que <strong>vous et vous seuls êtes responsables des interactions avec les autres membres de Warmshowers</strong></li>
</ol>
<br/><br/>
Voilà ... Vous pouvez maintenant remplir votre formulaire d\'inscription !
<hr/>
',
  'wsuser_become_available_notification_body' => 'Hi [user:fullname] - <br/><br/>Votre compte Warmshowers.org, [user], a été automatiquement mis à jour pour signifier que vous étiez disponibles pour accueillir des cyclistes. En effet, la date de retour que vous aviez spécifié pour votre retour est passée. Si vous n\'êtes toujours pas disponibles, merci de vous connecter sur Warmshowers.org et de mettre à jour votre compte. Vous pouvez vous connecter avec votre login et votre mot de passe sur <a href=\'https://fr.warmshowers.org\'>fr.warmshowers.org</a> ou <a href=\'[user:login_url]\'>cliquer sur ce lien</a> pour vous connecter automatiquement.<br/> <br/>Merci!<br/><br/>-Randy Fay<br/>Warmshowers.org Webmaster<br/>---<br/><a href=\'[user:url]\'>Profile link</a><br/><a href=\'[account-edit-url]\'>Edit profile link</a><br/><a href=\'[user:login_url]\'>Direct login link</a><br/>',
  'wsuser_become_available_notification_subject' => 'Vous êtes maintenant "disponibles" sur Warmshowers.org ([user:fullname])',
  'wsuser_notavailable_reminder_body' => 'Hi [user:fullname], <br/><br/>Votre compte warmshowers.org, [user], est marqué "indisponible" pour accueillir des cyclistes. Pouvez vous s\'il vous plaît vous connecter et renseigner la date à laquelle vous pensez être à nouveau disponible? Si vous ne savez pas quand vous rentrez ou quand vous pourrez de nouveau héberger des membres de warmshowers, renseignez simplement une date comme 2025-01-01. Vous pouvez la changer à tout moment. Vous pouvez vous connecter avec votre login et votre mot de passe sur <a href=\'https://fr.warmshowers.org\'>fr.warmshowers.org</a> ou <a href=\'[user:login_url]\'>cliquer sur ce lien pour vous connecter automatiquement</a><br/><br/>-Randy Fay<br/>Warmshowers.org Webmaster<br/>---<br/><a href=\'[user:url]\'>Profile link</a><br/><a href=\'[account-edit-url]\'>Edit profile link</a><br/><a href=\'[user:login_url]\'>Direct login link</a><br/>',
  'wsuser_notavailable_reminder_subject' => 'french version [user:fullname]',
);
  $realm_variables['language']['it'] = array(
  'contact_form_information' => '<p>Ti invitiamo a contattarci via email.</p>

<p>Se hai bisogno di assistenza nell’uso del sito, per favore <a href="/faq">leggi prima le FAQ </a>, e magari considera anche la lettura del <a href="http://www.warmshowers.org/forums/website-help-and-support">Forum di Aiuto</a> dove puoi scrivere sul tuo problema, altri/e possono cercare di aiutarti e la risposta che ricevi aiuterà altri/e a loro volta.</p>

<p>If you speak English it will help the admins a *lot* if you can ask your question in English. Thanks! </p>',
  'inactive_user_block_notify_text' => 'Hello %username,

  This automatic message is to notify you that your account on %sitename has been automatically disabled due to no activity for more than %period.

  Please visit us at %siteurl to have your account re-enabled.

Sincerely,
  %sitename team',
  'inactive_user_block_warn_text' => 'Hello %username,

  We haven\'t seen you at %sitename since %lastaccess, and we miss you!  This automatic message is to warn you that your account will be disabled in %period unless you come back and visit us before that time.

  Please visit us at %siteurl.

Sincerely,
  %sitename team',
  'inactive_user_delete_notify_text' => 'Hello %username,

  This automatic message is to notify you that your Warmshowers.org account has been automatically removed because you didn\'t log in for more than %period.

  Please visit us at %siteurl if you would like to create a new account.

Sincerely,
-Randy Fay
Warmshowers.org Webmaster',
  'inactive_user_delete_warn_text' => 'Hello %username,

Your account at Warmshowers.org will soon be removed (in %period) unless you log in to let us know you still want it. Please log into the site at http://www.warmshowers.org and update your account.  If you don\'t know your username or password that\'s OK - you can log in with your email address. If you don\'t know your password, please visit http://www.warmshowers.org/user/password to get a one-time login link emailed to you.

You haven\'t logged in %lastaccess.   This automatic message is to warn you that your account will be completely removed in %period unless you log into the site before that time.

  Please visit us at %siteurl.

Sincerely,
-Randy Fay
Warmshowers.org webmaster',
  'inactive_user_notify_text' => 'Ciao %username,

Non ti abbiamo più visto su Warmshowers.org da %lastaccess e ci manchi!  Chiediamo a tutti/e gli/le utenti di autenticarsi sul sito almeno una volta all’anno per controllare che le informazioni relative al loro profilo siano aggiornate e corrette e apprezzeremmo se anche tu aggiornassi il tuo profilo. O per lo meno *Entra* in maniera da far sapere al sistema che ricevi ancora i nostri messaggi a questo indirizzo email.

Per favore non aspettare troppo a lungo e aggiornati su  https://it.warmshowers.org.  Se non visiti il sito per un anno intero il tuo profilo verrà cancellato (ma riceverai diverse emails preventive di notifica).

Se non ti ricordi la password, vai su http://www.warmshowers.org/user/password, inserisci il tuo indirizzo email (%useremail) e riceverai un messaggio con un link unico che ti permetterà di ricrearne una.

Cordialmente,
-Randy Fay
Warmshowers.org webmaster

------
Admin Information
Search: http://www.warmshowers.org/search/wsuser/%useremail
Username: %username
Email: %useremail
',
  'user_mail_password_reset_body' => 'Ciao [user:name],

Tu (o qualcuno) ha richiesto un login unico per il tuo account su warmshowers.org. Se non hai richiesto questo servizio, ignora questo messaggio.

<a href="[user:one-time-login-url]">Per autenticarti e modificare la tua password, per favore clicca qui</a>. Se non è possibile aprire il collegamento, copia e incolla questa URL nel tuo motore di ricerca:

[user:one-time-login-url]

Questo è un login unico e può essere utilizzato una volta soltanto. Scade automaticamente dopo 24 ore se non utilizzato.

Dopo esserti autenticata/o, sarai reindirizzata/o su <a href="[user:edit-url]">la tua pagina personale, dove puoi modificare il tuo profilo</a> e dove potrai modificare la tua password.

Grazie,
-Randy Fay e il team di registrazione.
WarmShowers.org

Risoluzione dei problemi:

Se viene visualizzato il messaggio "Spiacenti – non hai accesso a questa pagina (error 403)", quasi certamente ci sono dei cookies disattivati sul tuo motore di ricerca.  Per configurare i cookies correttamente visita <a href="http://www.warmshowers.org/content/i-try-log-it-still-says-im-not-logged">nelle FAQ</a>.
Se non riesci a risolvere il problema, chiedi aiuto rispondendo a questa email.',
  'user_mail_password_reset_subject' => '[Warmshowers.org] Login unico per [user:name]',
  'user_mail_register_admin_created_body' => '[user:name],

A site administrator at !site has created an account for you. You may now log in to !login_uri using the following username and password:

username: [user:name]

You may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you will be redirected to [user:edit-url] so you can change your password.


--  !site team',
  'user_mail_register_admin_created_subject' => 'An administrator created an account for you at  !site',
  'user_mail_register_no_approval_required_body' => '[user:name],

Grazie per aver effettuato la registrazione su Warmshowers.org.

Ti preghiamo <a href="[user:one-time-login-url]">di cliccare qui</a> per registrare il tuo indirizzo email e abilitare il tuo account. (Se non è possibile aprire il collegamento, copia e incolla l\'indirizzo sul tuo motore di ricerca:

[user:one-time-login-url]

Dopo aver convalidato la tua registrazione, sarai autorizzata/o ad effettuare il login su !login_uri usando i seguenti nome utente e password (da te scelti):

nome utente: [user:name]
password: La password che hai scelto

Se ci sono problemi, rispondi a questa email e faremo del nostro meglio per venirti incontro.

Grazie,


--  !site team

--Ad uso esclusivo dell\'amministratore
Admin (modifica): <a href="[user:edit-url]">[user:edit-url]</a>',
  'user_mail_register_no_approval_required_subject' => '[Warmshowers.org] Confermare la registrazione su Warmshowers.org ([user:name])',
  'user_mail_register_pending_approval_body' => 'Welcome [user:name],

*** You must reply to this email to get access to the site! ***
*** Please reply saying something about yourself! ***
*** You must leave the message\'s subject and the rest of this message as they are. ***

( And when you reply - we need you to say "hi". You\'re not replying to a machine but to Randy and the rest of the team! )

*** You won\'t be able to log in until we enable your account
*** which is usually at least several hours after you send your reply.

Thank you for your registration at www.warmshowers.org.

Thank you for applying for an account, and for your hospitality to touring cyclists.

You need to acknowledge that you\'ve received this message by replying to it and saying something about yourself so we can correspond with you and try to make sure everybody on the list is a bona fide member.

I will enable your account after I receive your response.

(Minors - people not of legal age - are not allowed to use the site. So if you are under age, please do not continue.)

I\'ll be happy to approve your account. Your username and password (active after we correspond) are below.

Randy Fay
Warmshowers.org webmaster

AFTER your account has been authorized (after I receive your response and
activate your account), your username and password details will be:

username: [user:name]

AFTER your account is approved, you may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you can always change your password and other account details at  [user:edit-url]

Also: After you log in, please go to "my account" and the "map my location" tab and make sure your location shows correctly on the map. There are instructions on that page that will help you set it.

Thanks for signing up!

-Randy Fay and the registration team
Warmshowers.org

--
Administrator use only:
[user:edit-url]',
  'user_mail_register_pending_approval_subject' => 'Account info for  [user:name] at WarmShowers.org-You must reply and say something about yourself',
  'user_mail_status_activated_body' => 'Benvenuta/o [user:name],

Il tuo account su www.warmshowers.org è ora attivo.

Puoi adesso effettuare il login con i tuoi nome utente e password, o attraverso il link <a href="[user:one-time-login-url]"></a>.

Questo è un login unico e può essere utilizzato una volta soltanto.

Dopo esserti autenticata/o, sarai reindirizzata/o su <a href="[user:edit-url]">la tua pagina personale, dove puoi modificare il tuo profilo</a> e dove puoi cambiare la tua password se lo desideri.

Domande? Basta rispondere a questa mail.

Grazie,
-Randy Fay
Warmshowers.org webmaster

--
Ad uso esclusivo dell\'amministratore:
!uri',
  'user_mail_status_activated_subject' => '[Warmshowers.org] Il tuo account è stato creato con successo ([user:name])',
  'user_mail_status_blocked_body' => 'Ciao [user:name],

Il tuo account su www.warmshowers.org è stato bloccato.

Se hai delle domande, per favore rispondi a questa email.

Grazie,
Randy Fay
Warmshowers.org webmaster

Ad uso dell\'amministratore:
[user:edit-url]',
  'user_mail_status_blocked_subject' => '[Warmshowers.org] [user:name] account bloccato',
  'user_mail_status_deleted_body' => 'Ciao [user:name],

Il tuo account su www.warmshowers.org è stato cancellato, probabilmente come da te richiesto o perchè riconosciuto come invalido o spam.

Se hai delle domande, per favore rispondi a questa email.

Grazie,
Randy Fay
Warmshowers.org',
  'user_mail_status_deleted_subject' => '[Warmshowers.org] [user:name] account cancellato',
  'user_picture_guidelines' => 'Inviaci una foto di <i>Te</i>, con la tua bici o a casa tua. Si tratta di una scelta del tutto opzionale ma può rivelarsi divertente. Foto di questo tipo appaiono anche sulla pagina principale. <i>Se carichi una nuova foto e non diventa visibile entro pochi istanti, premi shift-refresh (shift+aggiorna) sul tuo motore di ricerca. (Tieni premuto il pulsante SHIFT e clicca su REFRESH/AGGIORNA.)</i>',
  'user_registration_help' => '<h3>Benvenuta/o! Per favore leggi qui prima di procedere</h3>
Siamo felici che tu abbia deciso di far parte della comunità Warm Showers! Fra pochi istanti potrai inserire i tuoi dettagli e subito dopo riceverai una email di conferma – dovrai cliccare sul collegamento per attivare il tuo account. Ma attenzione, perchè con tutti i filtri antispam che abbiamo oggigiorno, potresti non ricevere mai questa email se prima non aggiungi Warm Showers alla Lista Bianca nella casella dello spam! Quindi …

<ol><li>
Per favore, prima di riempire il formulario, aggiungi l\'indirizzo wsl@warmshowers.org alla tua Lista Bianca. Se non provvedi subito, potresti non ricevere la email di conferma che ti invieremo con la tua nuova password e non potrai quindi creare un account sul nostro sito.</li>
<li>Per favore dai un\'occhiata a <a href="/the_rules" target="_blank">the Rules</a> e ricordati che puoi fare riferimento a questa pagina ogni volta che hai dei dubbi.</li>
<li>Ricorda che nel momento in cui crei un account, ti stai impegnando a offire ospitalità, sebbene non nell\'immediato. Questo è un sito di ospitalità reciproca. (Se al momento sei in viaggio o per qualche altra ragione non puoi offrire ospitalità, puoi spuntare la casella "Al momento non disponibile" e in futuro potrai togliere la spunta quando sarai disponibile.)</li>
<li>I minori (persone che non hanno ancora raggiunto la maggiore età) non sono autorizzati ad usare questo sito, quindi, se hai meno di 18 anni, per favore non continuare oltre.</li>
<li>Ricorda che puoi consultare le <a href="/faq" target="_blank">Domande Frequenti</a>, dove ci sono le risposte a problemi comunemente riscontrati, quindi per prima cosa fai rimerimento lì se hai dei problemi di autenticazione.</li>
<li>Non sei sicura/o di chi potrà vedere le informazioni relative al tuo account? Questo sito è organizzato in maniera che le informazioni che inserisci non sono visibili al mondo esterno; solo i membri potranno vederle, una volta essersi autenticate/i.  Il tuo indirizzo email è invece sempre invisibile, anche ai membri.</li>
<li>Anche la tua sicurezza è molto importante per noi.  Per favore leggi la sezione <a href="/sicurezza-personale">la pagina di Sicurezza Personale page</a> per saperne di più a questo riguardo.</li>
<li>Il nostro <a target="_blank" href="/privacy">Privacy Policy</a> ti ricorda che il tuo profilo utente è visibile solo  agli/alle altri/e utenti registrati/e e solo questi/e possono contattarti.</li>
<li>Registrandoti, aderisci ai nostri <a target="_blank" href="/terms">Termini di Utilizzo</a dove si dichiara che  <strong>tu e tu solo/a sei responsabile per qualunque interazione con altri/e utenti Warmshowers </strong></li>
</ol>
<br/><br/>
OK, è tutto per ora! Adesso prosegui con la registrazione.
<hr/>',
  'wsuser_become_available_notification_body' => 'Ciao [user:fullname] - <br/><br/>Il tuo profilo Warmshowers.org, [user], è stato automaticamente aggiornato, dichiarandoti disponibile ad offrire ospitalità ad altri/e utenti, in conformità con la data da te indicata per questo cambiamento. Se ancora non sei disponibile, per favore visita il tuo profilo su Warmshowers.org e aggiornalo con una data diversa. Puoi autenticarti semplicemente andando sul sito o <a href=\'[user:login_url]\'>cliccando su questo link</a>. Grazie!<br/><br/>-Randy Fay<br/>Amministratore di Warmshowers.org <br/>---<br/><a href=\'[user:url]\'>Collegamento al Il mio profilo</a><br/><a href=\'[account-edit-url]\'>Collegamento a Modifica profilo</a><br/><a href=\'[user:login_url]\'>Log in diretto</a><br/>',
  'wsuser_become_available_notification_subject' => 'Sei ora contrassegnato/a come disponibile ad offrire ospitalità su Warmshowers.org ([user:fullname])',
  'wsuser_notavailable_reminder_body' => 'Ciao [user:fullname], <br/><br/>Il tuo profilo su warmshowers.org, [user], è contrassegnato come non disponibile ad offrire ospitalità. Per favore accedi al tuo profilo e imposta la data in cui prevedi di ritornare ad esserlo. Se non sei sicura/o quando questo potrà verificarsi, imposta una data molto lontana nel tempo, come il 01-01-2025. Puoi cambiarla in ogni momento. Puoi autenticarti con i tuoi nome utente e password automaticamente, cliccando qui <a href=\'https://it.warmshowers.org\'>it.warmshowers.org</a> o <a href=\'[user:login_url]\'>cliccando qui per il Log in diretto</a><br/><br/>-Randy Fay<br/>Amministratore di Warmshowers.org <br/>---<br/><a href=\'[user:url]\'>Collegamento al Il mio profilo</a><br/><a href=\'[account-edit-url]\'>Collegamento a Modifica profilo</a><br/><a href=\'[user:login_url]\'>Log in diretto</a><br/>',
  'wsuser_notavailable_reminder_subject' => 'IL tuo profilo su Warmshowers è contrassegnato come non disponibile. Per favore aggiorna la tua data di ritorno. ([user:fullname])',
);
  $realm_variables['language']['ja'] = array(
  'contact_form_information' => '<p>You are welcome to contact us here via email in English.</p>
<p>Eメール（英語）でのコンタクトを歓迎します</p>
<p>もしこのサイトを使うにあたり、ヘルプが必要な場合は、どうか <a href="/faq">よくある質問を最初に</a>ご覧下さい。そして、どうか<a href="http://www.warmshowers.org/forums/website-help-and-support">ヘルプフォーラム</a>を良く読み、投稿してください。他の人の助言が得られるかも知れませんし、その質問への回答が別な人を助ける事にもなります。</p>

<p>If you speak English it will help the admins a *lot* if you can ask your question in English. Thanks! </p>',
  'user_mail_password_reset_body' => '[user:name]さん、こんにちは。

あなた（もしくは誰か）が、あなたのwarmshowers.orgへのワンタイムログインを依頼しました。もし依頼されたのがあなたでない場合は、このメッセージを無視して下さい。

<a href="[user:one-time-login-url]">ここをクリックしてログインし、パスワードをリセット</a>して下さい。もしこのリンクをクリックできない場合は、以下のURLをコピーし、ブラウザにペースとして下さい:

[user:one-time-login-url]

これはワンタイムログインなので一度しか使用できません。このアドレスは一日で期限切れとなり、もし使用されない場合には何も起きません。

ログイン後、あなたは<a href="[user:edit-url]">あなたのアカウントプロファイル編集ページ</a>にリダイレクトされますので、そこでパスワードを変更できます。

ありがとうございます。

-WarmShowers.org、Randy Fay および登録チームより

トラブルシューティング:

もし、"Sorry - you don\'t have access to that page (error 403)"と表示されるなら、恐らくあなたがブラウザのクッキーを使用しない設定にしていることが原因です。クッキーを正しく設定させるには <a href="http://www.warmshowers.org/content/i-try-log-it-still-says-im-not-logged">良くある質問のこの項目</a>をご覧下さい。

If you can\'t solve this problem, please ask for help. Reply to this message in English and ask for help.
もし、問題を解決できない場合は、このメールに英語で返信して質問して下さい',
  'user_mail_password_reset_subject' => '[Warmshowers.org] [user:name]さんのためのワンタイムログインのリンク',
  'user_mail_register_admin_created_body' => '[user:name],

A site administrator at !site has created an account for you. You may now log in to !login_uri using the following username and password:

username: [user:name]

You may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you will be redirected to [user:edit-url] so you can change your password.


--  !site team',
  'user_mail_register_admin_created_subject' => 'An administrator created an account for you at  !site',
  'user_mail_register_no_approval_required_body' => '[user:name],

Warmshowers.orgにご登録いただき、ありがとうございます。

<a href="[user:one-time-login-url]">ここをクリック</a>してあなたのメールアドレスとアカウントを有効化して下さい。（もし上のリンクをクリックできない場合は、以下のリンクをコピーし、ブラウザにペーストして下さい）

[user:one-time-login-url]

上のリンクでアカウントを有効化すると、下記のユーザー名と入力されたパスワードで!login_uriにログインできるようになります:

ユーザー名: [user:name]
パスワード: あなたが入力したパスワード

If you have any questions, you can reply to this email in English and we\'ll do our best to help you out.
もし何か質問がある場合は、このメールに英語で返信をして下さい。ご期待に応えられるよう、お手伝いいたします。

ありがとうございます。


--  !site チーム

--以下は管理用リンク
Admin (edit): <a href="[user:edit-url]">[user:edit-url]</a>',
  'user_mail_register_no_approval_required_subject' => '[Warmshowers.org] Warmshowers.orgのアカウントを有効化する',
  'user_mail_register_pending_approval_body' => 'Welcome [user:name],

*** You must reply to this email to get access to the site! ***
*** Please reply saying something about yourself! ***
*** You must leave the message\'s subject and the rest of this message as they are. ***

( And when you reply - we need you to say "hi". You\'re not replying to a machine but to Randy and the rest of the team! )

*** You won\'t be able to log in until we enable your account
*** which is usually at least several hours after you send your reply.

Thank you for your registration at www.warmshowers.org.

Thank you for applying for an account, and for your hospitality to touring cyclists.

You need to acknowledge that you\'ve received this message by replying to it and saying something about yourself so we can correspond with you and try to make sure everybody on the list is a bona fide member.

I will enable your account after I receive your response.

(Minors - people not of legal age - are not allowed to use the site. So if you are under age, please do not continue.)

I\'ll be happy to approve your account. Your username and password (active after we correspond) are below.

Randy Fay
Warmshowers.org webmaster

AFTER your account has been authorized (after I receive your response and
activate your account), your username and password details will be:

username: [user:name]

AFTER your account is approved, you may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you can always change your password and other account details at  [user:edit-url]

Also: After you log in, please go to "my account" and the "map my location" tab and make sure your location shows correctly on the map. There are instructions on that page that will help you set it.

Thanks for signing up!

-Randy Fay and the registration team
Warmshowers.org

--
Administrator use only:
[user:edit-url]',
  'user_mail_register_pending_approval_subject' => 'Account info for  [user:name] at WarmShowers.org-You must reply and say something about yourself',
  'user_mail_status_activated_body' => '[user:name]さん、ようこそ。

あなたのwww.warmshowers.orgのアカウントは有効化されました。

あなたはご自身のユーザー名と既にお持ちのパスワードでログインできます。
あるいは<a href="[user:one-time-login-url]">ここをクリック</a>することでもログインが可能です。
（これはワンタイムログインなので、一回だけ使用できます）

ログイン後、あなたは<a href="[user:edit-url]">アカウントプロファイル編集ページ</a>にリダイレクトされます。そこで必要に応じパスワードを変更できます。

Any questions? Just reply to this message in English.
何かご質問があれば、このメールに英語で返信して下さい。

ありがとうございます。
-Warmshowers.orgウェブマスターRandy Fayより

--
以下は管理用リンク
!uri',
  'user_mail_status_activated_subject' => '[Warmshowers.org] あなたのアカウントは承認されました',
  'user_mail_status_blocked_body' => 'Hello [user:name],

Your account on www.warmshowers.org has been blocked.

If you have any questions, please reply to this email.

Thanks,
Randy Fay
Warmshowers.org webmaster

Admin use:
[user:edit-url]',
  'user_mail_status_blocked_subject' => 'Warmshowers.org: [user:name] account blocked',
  'user_mail_status_deleted_body' => '[user:name]さん、こんにちは。

あなたのwww.warmshowers.orgのアカウントは削除されました。これは恐らくは
ご自身の依頼によるものです。（スパムによる不正アクセスの可能性もあります）

If you have any questions, please reply to this email in English.
もし何か質問がある場合は、このメールに英語で返信して下さい。

ありがとうございます。
Warmshowers.orgのRandy Fayより
[Warmshowers.org] [user:name]さんのアカウントは削除されました
[user:name]さん、こんにちは。
',
  'user_mail_status_deleted_subject' => '[Warmshowers.org] [user:name]さんのアカウントは削除されました',
  'user_picture_guidelines' => '私たちは、あなたが、地元でサイクリングしているような<i>あなた</i>の写真を望んでいます。これは完全に任意のオプションですが、あれば楽しいものです。これはフロントページのランダムな写真にも表示されます。 <i>もし新しい写真をアップロードした場合、すぐには表示されません。ブラウザでシフトキーを押しながら再読み込みのボタンを押してみて下さい。</i>',
  'user_registration_help' => '<h3>ようこそ! 以下を良く読んでからお進みください</h3>
Warm Showersに登録しようとして頂き、大変うれしく思います!
ここでは、まずあなたに、あなたの情報を入力していただいた後、こちらから確認メールをお送りします。次に、そのメールのリンクをクリックして頂く必要があります。しかし、最近のスパムフィルタは何十億とありますので、もし私たちのアドレスをあなたのスパム・ホワイトリストに追加しないと、あなたは確認メールを受け取れない場合があるかも知れません。この場合、あなたは永遠にあなたのアカウントを有効にできないことになってしまいます....
<ol><li>
このフォームを送信する前に、今すぐwsl@warmshowers.orgをスパム・ホワイトリストに追加してください。多くのメールプロバイダにおいて、どう設定するかは<a href="/spam_filters" target="_blank">こちら</a>に説明があります。
もしあなたがこれを行わない場合、あなたはパスワードを有効化するための確認メールを受け取れないかも知れず、結果として登録が完了しないかもしれません。</li>
<li><a href="/the_rules" target="_blank">規則</a>のページを見て確認してください。不確かな場合は必ずそれらを見返し、チェックしてください。</li>
<li>
登録する、という事は、あなたが(いつか)ホストとして受け入れを申し出ることを意味する、ということを覚えておいて下さい。
ここは、 互いに受け入れ・もてなしを行うサイトです。（もしあなたが旅行中だったり、何らかの理由で現在受け入れを行うことができない場合は、「今は対応できません」というチェックボックスにチェックを入れておき、将来そのチェックを外すことができます）</li>
<li>
未成年者(法定年齢に達しない人)はこのサイトを使用することが認められません。もしあなたが、未成年の場合は、これ以上続けないで下さい。</li>
<li>
何か問題があった場合、解決できるよう<a href="/faq" target="_blank">良くある質問</a>ページが準備されていることを覚えておいて下さい。そしてもしサイトにログインするのに問題が生じた際は忘れずにチェックしてみて下さい。</li>
<li>
誰がここに置く情報を見ることができるか、について関心を持っていますか? このサイトはあなたのプロファイルが外の世界から見ることができないように作られています。それはログインした後にメンバーだけが見ることができます。メールアドレスは他のメンバーにも、常に隠されています。
</li>
<li>
あなたの個人としての安全（パーソナルセキュリティー）もまた、私たちに取って大変重要です。どうか<a href="/personal-security">パーソナルセキュリティーページ</a>で、より詳しい情報を読んください。</li>
<li>私たちの<a target="_blank" href="/privacy">プライバシーポリシー</a>には
あなたのメンバープロファイルが他のメンバーのみに対してのみ開示されており、メンバーのみがあなたにコンタクト可能であることが書かれています。
</li>
<li>登録をすることにより、あなたは私たちの<a target="_blank" href="/terms">利用条件</a>（<strong>「あなたは、すべての他のWarmshowersメンバーとの間に発生する相互作用について責任を持つ」</strong>という点を強調しています）</li>に同意したことになります。
</ol>
<br/><br/>
これですべてです!
以下の登録フォームの入力を続けて下さい。
<hr/>',
  'wsuser_become_available_notification_body' => 'Hi [user:fullname] - <br/><br/>Your Warmshowers.org account, [user], has been automatically updated to show you as "available to host" because the date that you specified you would become available after has passed. If you are still unavailable to host please log into Warmshowers.org and update your account. You can log in by visiting the site or just <a href=\'[user:login_url]\'>clicking this link</a>. Thanks!<br/><br/>-Randy Fay<br/>Warmshowers.org Webmaster<br/>---<br/><a href=\'[user:url]\'>Profile link</a><br/><a href=\'[account-edit-url]\'>Edit profile link</a><br/><a href=\'[user:login_url]\'>Direct login link</a><br/>',
  'wsuser_become_available_notification_subject' => 'You are now listed as available to host on Warmshowers.org ([user:fullname])',
  'wsuser_notavailable_reminder_body' => 'Hi [user:fullname], <br/><br/>Your warmshowers.org account, [user], is marked as unavailable to host. Could you please log in and set the date that you expect to return? If you don\'t know when or if you\'ll be able to host, just put in a date like 2025-01-01. You can change it at any time. You can log in with your username and password at <a href=\'https://ja.warmshowers.org\'>ja.warmshowers.org</a> or <a href=\'[user:login_url]\'>click this link to log in automatically</a><br/><br/>-Randy Fay<br/>Warmshowers.org Webmaster<br/>---<br/><a href=\'[user:url]\'>Profile link</a><br/><a href=\'[account-edit-url]\'>Edit profile link</a><br/><a href=\'[user:login_url]\'>Direct login link</a><br/>',
  'wsuser_notavailable_reminder_subject' => 'Your Warmshowers account is marked unavailable. Please update your return date. ([user:fullname])',
);
  $realm_variables['language']['pl'] = array(
  'comment_notify_default_mailtext' => 'Witaj [comment-subscribed:author:fullname],

[comment:author:fullname] skomentował: "[comment:node:title]":

----
[comment:title]
[comment:body]
----

Możesz zobaczyć i odpowiedzieć na komentarz, <a href="[comment:url]">klikając tutaj</a>. (Prosimy nie odpowiadać za pośrednictwem poczty elektronicznej, ponieważ e-maile te trafiają do zarządzających stroną.)

Możesz zrezygnować z otrzymywania wiadomości e-mail, gdy ktoś odpowie na ten wpis, <a href="[comment-subscribed:unsubscribe-url]">klikając tutaj</a>.

- Zespół Warmshowers.org
',
  'contact_form_information' => '<p>Zapraszamy do kontaktu z nami poprzez e-mail.</p>

<p>Jeśli potrzebujesz pomocy, prosimy najpierw <a href="/faq">przeczytać listę FAQ</a>, a następnie napisać na <a href="http://www.warmshowers.org/forums/website-help-and-support">Forum</a>, gdzie inni uzytkownicy mogą udzielić Ci pomocy, a odpowiedzi na pytania mogą także pomóc innym.</p>

<p>If you speak English it will help the admins a *lot* if you can ask your question in English. Thanks! </p>',
  'inactive_user_block_notify_text' => 'Hello %username,

  This automatic message is to notify you that your account on %sitename has been automatically disabled due to no activity for more than %period.

  Please visit us at %siteurl to have your account re-enabled.

Sincerely,
  %sitename team',
  'inactive_user_block_warn_text' => 'Hello %username,

  We haven\'t seen you at %sitename since %lastaccess, and we miss you!  This automatic message is to warn you that your account will be disabled in %period unless you come back and visit us before that time.

  Please visit us at %siteurl.

Sincerely,
  %sitename team',
  'inactive_user_delete_notify_text' => 'Hello %username,

  This automatic message is to notify you that your Warmshowers.org account has been automatically removed because you didn\'t log in for more than %period.

  Please visit us at %siteurl if you would like to create a new account.

Sincerely,
-Randy Fay
Warmshowers.org Webmaster',
  'inactive_user_delete_warn_text' => 'Hello %username,

Your account at Warmshowers.org will soon be removed (in %period) unless you log in to let us know you still want it. Please log into the site at http://www.warmshowers.org and update your account.  If you don\'t know your username or password that\'s OK - you can log in with your email address. If you don\'t know your password, please visit http://www.warmshowers.org/user/password to get a one-time login link emailed to you.

You haven\'t logged in %lastaccess.   This automatic message is to warn you that your account will be completely removed in %period unless you log into the site before that time.

  Please visit us at %siteurl.

Sincerely,
-Randy Fay
Warmshowers.org webmaster',
  'inactive_user_notify_text' => 'Witaj %username,

Nie logowałeś się na stronę warmshowers.org od %lastaccess. Brakuje nam Ciebie! Prosimy wszystkich, aby logowali się przynajmniej raz w roku, aby upewnić się, że ich dane są prawidłowe, więc będziemy wdzięczni, jeśli zalogujesz się i zaktualizujesz swoje konto. Albo przynajmniej *zaloguj się*, tak aby system wiedział, że wciąż sprawdzasz tę skrzynkę mailową.

Zaktualizuj informacje wkrótce na %siteurl.  Jeśli nie zalogujesz się przez okres pełnego roku kalendarzowego, Twoje konto zostanie w końcu usunięte (otrzymasz jednak wcześniej kilka powiadomień).

Jeśli zapomniałeś hasła, przejdź na http://www.warmshowers.org/user/password i wprowadź swój adres e-mail (%useremail), aby otrzymać jednorazowy link do logowania.

Z poważaniem,
-Randy Fay
Webmaster Warmshowers.org

------
Admin Information
Wyszukiwanie: http://www.warmshowers.org/search/wsuser/%useremail
Nazwa użytkownika: %username
E-mail: %useremail',
  'node_notify_default_mailtext' => 'Witaj [comment:node:author:fullname],

Otrzymałeś komentarz w sprawie: "[comment:node:title]":

----
[comment:title]
[comment:body]
----

Możesz zobaczyć ten komentarz, <a href="[comment:url]">klikając tutaj</a>.  (Prosimy nie odpowiadać za pośrednictwem poczty elektronicznej, ponieważ e-maile te trafiają do zarządzających stroną.)

Będziesz otrzymywał podobne e-maile informujące o wszystkich odpowiedziach na Twoje wpisy.

- Zespół Warmshowers',
  'user_mail_password_reset_body' => 'Witaj [user:name],

Ty (lub ktoś inny) poprosił o wysłanie jednorazowego przypomnienia loginu do konta na warmshowers.org Jeśli nie byłeś to Ty, możesz po prostu zignorować tę wiadomość.

Kliknij <a href="[user:one-time-login-url]">tutaj, aby się zalogować i zresetować hasło</a>. Jeśli nie możesz kliknąć w link, skopiuj go i wklej do przeglądarki:

[user:one-time-login-url]

Jest to jednorazowy login, może więc być użyty tylko raz. Login wygaśnie po jednym dniu. Możesz go w ogóle nie użyć, nic się wtedy nie stanie.

Po zalogowaniu zostaniesz przeniesiony na <a href="[user:edit-url]">swoją stronę profilową</a>, abyś mógł zmienić hasło.

Dziękujemy,
- Randy Fay i Zespół Rejestracji
WarmShowers.org

Rozwiązywanie problemów:

Jeśli wyświetla Ci się „Niestety nie masz dostępu do tej strony (błąd 403)", najprawdopodobniej oznacza to, że masz wyłączoną obsługę plików cookies w przeglądarce.  Aby poprawnie skonfigurować te pliki przeczytaj <a href="http://www.warmshowers.org/content/i-try-log-it-still-says-im-not-logged">ten artykuł na stronie FAQ</a>.

Jeśli nie potrafisz rozwiązać tego problemu, poproś o pomoc odpowiadając na tę wiadomość.',
  'user_mail_password_reset_subject' => '[Warmshowers.org] Jednorazowy link do logowania dla [user:name]',
  'user_mail_register_admin_created_body' => '[user:name],

A site administrator at !site has created an account for you. You may now log in to !login_uri using the following username and password:

username: [user:name]

You may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you will be redirected to [user:edit-url] so you can change your password.


--  !site team',
  'user_mail_register_admin_created_subject' => 'An administrator created an account for you at  !site',
  'user_mail_register_no_approval_required_body' => '[user:name],
Dziękujemy za rejestrację na Warmshowers.org.

Kliknij <a href="[user:one-time-login-url]">tutaj</a>, aby potwierdzić swój adres e-mail i uaktywnić konto. Jeśli nie możesz kliknąć w link, skopiuj go i wklej do przeglądarki:

[user:one-time-login-url]

Po potwierdzeniu kliknięciem w link powyżej będziesz mógł się zalogować na !login_uri wykorzystując następujące dane, które podałeś:

nazwa użytkownika: [user:name]
hasło: hasło, które podałeś

Jeśli masz jakiekolwiek pytania, możesz odpowiedzieć na tę wiadomość, a my dołożymy wszelkich starań, aby Ci pomóc.

Dziękujemy,


-- !site team
',
  'user_mail_register_no_approval_required_subject' => ' [Warmshowers.org] Weryfikacja konta na Warmshowers.org',
  'user_mail_register_pending_approval_body' => 'Welcome [user:name],

*** You must reply to this email to get access to the site! ***
*** Please reply saying something about yourself! ***
*** You must leave the message\'s subject and the rest of this message as they are. ***

( And when you reply - we need you to say "hi". You\'re not replying to a machine but to Randy and the rest of the team! )

*** You won\'t be able to log in until we enable your account
*** which is usually at least several hours after you send your reply.

Thank you for your registration at www.warmshowers.org.

Thank you for applying for an account, and for your hospitality to touring cyclists.

You need to acknowledge that you\'ve received this message by replying to it and saying something about yourself so we can correspond with you and try to make sure everybody on the list is a bona fide member.

I will enable your account after I receive your response.

(Minors - people not of legal age - are not allowed to use the site. So if you are under age, please do not continue.)

I\'ll be happy to approve your account. Your username and password (active after we correspond) are below.

Randy Fay
Warmshowers.org webmaster

AFTER your account has been authorized (after I receive your response and
activate your account), your username and password details will be:

username: [user:name]

AFTER your account is approved, you may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you can always change your password and other account details at  [user:edit-url]

Also: After you log in, please go to "my account" and the "map my location" tab and make sure your location shows correctly on the map. There are instructions on that page that will help you set it.

Thanks for signing up!

-Randy Fay and the registration team
Warmshowers.org

--
Administrator use only:
[user:edit-url]',
  'user_mail_register_pending_approval_subject' => 'Account info for  [user:name] at WarmShowers.org-You must reply and say something about yourself',
  'user_mail_status_activated_body' => 'Witaj [user:name],

Twoje konto na www.warmshowers.org zostało aktywowane.

Możesz zalogować się przy użyciu nazwy użytkownika i hasła, które już posiadasz lub możesz zalogować się <a href="[user:one-time-login-url]">klikając tutaj</a>.

Jest to jednorazowy login, może więc być użyty tylko raz.

Po zalogowaniu zostaniesz przeniesiony na <a href="[user:edit-url]">swoją stronę profilową</a>, abyś mógł zmienić hasło jeśli chcesz.

Masz pytania? Po prostu odpowiedz na tę wiadomość.

Dziękujemy,
-Randy Fay
Webmaster Warmshowers.org

--
Admin use only:
!uri',
  'user_mail_status_activated_subject' => '[Warmshowers.org] Twoje konto zostało potwierdzone',
  'user_mail_status_blocked_body' => 'Hello [user:name],

Your account on www.warmshowers.org has been blocked.

If you have any questions, please reply to this email.

Thanks,
Randy Fay
Warmshowers.org webmaster

Admin use:
[user:edit-url]',
  'user_mail_status_blocked_subject' => 'Warmshowers.org: [user:name] account blocked',
  'user_mail_status_deleted_body' => 'Witaj [user:name],

Twoje konto na www.warmshowers.org zostało usunięte, najprawdopodobniej na Twoją prośbę lub w wyniku podejrzenia, iż jest ono nieprawidłowym kontem bądź spamem.

Jeśli masz jakiekolwiek pytania, prosimy o odpowiedź na tę wiadomość.

Dziękujemy,
Randy Fay
Warmshowers.org',
  'user_mail_status_deleted_subject' => '[Warmshowers.org] [user:name] konto usunięte',
  'user_picture_guidelines' => 'Chcielibyśmy, abyś dodał <i>swoje</i> zdjęcie, na rowerze albo w domu,  w którym przyjmować będziesz gości. Jest to oczywiście opcja całkowicie dobrowolna, ale naszym zdaniem całkiem przyjemna. Losowe zdjęcia ukazują się także na pierwszej stronie. <i>Jeśli załadowałeś nowe zdjęcie, a nie pojawiło się ono od razu na stronie, odśwież widok w przeglądarce (shift + odśwież). (Naciśnij guzik SHIFT na klawiaturze, a następnie przycisk ODŚWIEŻ w przeglądarce.)</i>',
  'user_registration_help' => '<h3>Witaj! Przeczytaj zanim pójdziesz dalej</h3>
Cieszymy się, że chcesz dołączyć do społeczności Warm Showers! Za chwilę poprosimy Cię, abyś wprowadził informacje o sobie, a następnie wyślemy Ci e-mail potwierdzający - należy kliknąć w link w mailu.  W dzisiejszych czasach milionów filtrów antyspamowych możesz jednak nie dostać naszej wiadomości, jeśli nie dodasz nas do listy zaufanych odbiorców, a wtedy Twoje konto nigdy nie zostanie stworzone. Więc ....

<ol><li>
Teraz, przed wysłaniem tego formularza, dodaj wsl@warmshowers.org do listy zaufanych odbiorców. Jeśli tego nie zrobisz możesz nie otrzymać naszej wiadomości potwierdzającej zawierającej hasło, więc Twoje konto nie zostanie stworzone.</li>
<li>Prosimy zapoznać się z <a href="/the_rules" target="_blank">zasadami</a>, do których zawsze możesz się odwołać w razie wątpliwości.</li>
<li>Prosimy pamiętać, że fakt dołączenia do społeczności oznacza, że zamierzasz oferować gościnę, przynajmniej kiedyś w przyszłości. Ta strona bazuje na wzajemnej gościnie. (Jeśli jesteś akurat w drodze albo z innych powodów nie możesz teraz przyjmować do siebie nikogo, zaznacz opcję "aktualnie jestem niedostępny", którą zawsze możesz w przyszłości odznaczyć.)</li>
<li>Osoby niepełnoletnie nie mogą korzystać z tej witryny, prosimy więc nie kontynuować rejestracji jeśli jesteś osobą nieletnią.</li>
<li>Prosimy pamiętać o naszej stronie z <a href="/faq" target="_blank">Najczęściej zadawanymi pytaniami</a>, która może pomóc rozwiązać wiele problemów, których możesz doświadczyć, np. tych związanych z logowaniem się na stronę.</li>
<li>Obawiasz się, kto będzie miał dostęp do informacji, które tu zamieszczasz?  Strona jest skonstruowana w taki sposób, że informacje na Twoim profilu nie są widoczne publicznie. Tylko inni członkowie społeczności będą mogli je wyświetlić po zalogowaniu się.  Dodatkowo, Twój adres e-mail pozostaje zawsze ukryty, nawet dla innych członków.</li>
<li>Twoje bezpieczeństwo osobiste jest bardzo ważne także dla nas.  Zapoznaj się ze stroną <a href="/personal-security"> bezpieczeństwo osobiste</a>, aby przeczytać więcej na ten temat.</li>
<li>Nasza <a target="_blank" href="/privacy">polityka prywatności</a> przypomina, że Twój profil widoczny jest tylko dla innych członków społeczności i że tylko członkowie mogą się z Tobą kontaktować.</li>
<li>Rejestrując się wyrażasz zgodę na nasze <a target="_blank" href="/terms">warunki użytkowania serwisu</a>, które podkreślają, że <strong> Ty i tylko Ty jesteś odpowiedzialny za każdą interakcję z innymi członkami społeczności Warm Showers</strong></li>
</ol>
<br/><br/>
To wszystko! Prosimy teraz, abyś kontynuował i wypełnił formularz rejestracyjny.
<hr/>',
  'wsuser_become_available_notification_body' => 'Witaj [user:fullname], <br/><br/>Twoje konto Warmshowers.org - [user], zostało automatycznie uaktualnione i wyświetla teraz informację "Mogę przyjmować gości", ponieważ Twoja data dostępności ustawiona przez Ciebie wcześniej, właśnie minęła. Jeśli nadal nie możesz przyjmować nikogo, zaloguj się na Warmshowers.org i uaktualnij informacje na swoim koncie. Możesz zalogować się, odwiedzając witrynę lub po prostu <a href="[user:login_url]">klikając na ten link</a>. Dziękujemy!<br/><br/>-Randy Fay<br/> Webmaster Warmshowers.org <br/>---<br/><a href="[user:url]">Link do profilu</a><br/><a href="[account-edit-url]">Link do edycji profilu</a><br/><a href="[user:login_url]"> Link do bezpośredniego logowania</a><br/>',
  'wsuser_become_available_notification_subject' => 'Wyświetlasz się teraz jako dostępny dla gości na Warmshowers.org ([user:fullname])',
  'wsuser_notavailable_reminder_body' => 'Witaj [user:fullname], <br/><br/>Twoje konto Warmshowers.org - [user], jest oznaczone jako niedostępne dla gości. Czy możesz się zalogować i ustawić datę swojego spodziewanego powrotu? Jeśli nie znasz jej jeszcze lub wiesz, że dalej nie będziesz mógł nikogo gościć, wpisz po prostu bardzo odległą datę, np. 01.01.2025. Możesz ją zmienić w każdej chwili. Możesz zalogować się przy pomocy swojego konta użytkownika i hasła na <a href="https://pl.warmshowers.org">pl.warmshowers.org</a> lub<a href="[user:login_url]"> kliknąć ten link, aby zalogować się automatycznie</a><br/><br/>-Randy Fay<br/> Webmaster Warmshowers.org <br/>---<br/><a href="[user:url]">Link do profilu</a><br/><a href="[account-edit-url]">Link do edycji profilu</a><br/><a href="[user:login_url]"> Link do bezpośredniego logowania</a><br/>',
  'wsuser_notavailable_reminder_subject' => 'Twoje konto na Warmshowers.org wyświetla się jako niedostępne. Prosimy uaktualnić datę powrotu. ([user:fullname])',
);
  $realm_variables['language']['pt-br'] = array(
  'anonymous' => 'anônimo',
  'contact_form_information' => '<p>Snta-se à vontade para contatar-nos aqui por e-mail.</p>
<p>Se precisar de ajuda para usar o site, <a href="/faq">primeiro leia o FAQ (Perguntas Frequentes) </a>, se não resolver acesse o <a href="http://www.warmshowers.org/forums/website-help-and-support">Forum de Ajuda</a> e publique lá para que os outros possam ajudá-lo, assim a resposta pode ajudar outros também. </p>

<p>If you speak English it will help the admins a *lot* if you can ask your question in English. Thanks! </p>',
  'site_footer' => ' Hospitalidade para cicloviajantes  ao redor do mundo<br/>
<a href="/">Início</a>  <a href="/map">Mapa</a>',
  'site_mission' => '<a href="http://pt.warmshowers.org">warmshowers.org</a>: hospitalidade (e banho quente!) para cicloviajantes',
  'site_slogan' => 'Hospitalidade para cicloviajantes',
  'user_mail_password_reset_body' => 'Olá [user:name],

Você (ou alguém) solicitou uma nova senha para sua conta no WarmShowers.org. Se não foi você que fez a solicitação, você pode simplesmente ignorar esta mensagem.

Por favor <a href="[user:one-time-login-url]">clique aqui para entrar</a> e redefinir sua senha.

Este link só poderá ser usado uma vez e expira depois de um dia. Caso você não o use dentro do prazo não há problema, basta solicitar outro no WarmShowers.org

Ao clicar no link e efetuar login, você será redirecionado à <a href="[user:edit-url]">página de edição de seu cadastro</a> para poder trocar sua senha.

Gratos,

-Randy Fay e a equipe de inscrição
WarmShowers.org

Solução de Problemas:

Se você receber a mensagem "Acesso restrito (Erro 403)" muito provavelmente seu navegador está com os cookies desativados. Retirado das Perguntas Frequentes (FAQ - http://pt.warmshowers.org/faq):


Ao clicar no link para mudar a senha, recebo a mensagem "acesso negado"

Nesta situação você efetua login e não recebe nenhuma resposta de erro, porém o sistema diz que você não está conectado. Este problema normalmente acontece porque os "cookies" do navegador estão desabilitados ou não estão funcionando. Para entrar no site você tem que permitir os "cookies". Eles não são uma ameaça ao seeu computador e são exigidos pela maioria dos sites da internet para funcionar corretamente. Para permitir os cookies, faça uma (ou mais) das opções abaixo:

No Internet Explorer, escolha Ferramentas->Opções da Internet->Privacidade e configure para qualquer opção mais permissiva que "bloquear todos os cookies

Clique em "sites" na mesma caixa de diálogo e permita cookies para warmshowers.org

Em alguns casos, já vimos o IE "se comportar mal" e será preciso apagar todos os cookies (Ferramentas->Opções da Internet->Geral->Apagar Cookies)

Experimente o excelente navegador firefox como alternativa. Se você não tem permissão para instalar software no computador, experimente o Firefox Portable Edition, que não precisa ser instalado (Randy leva ele em um pendrive quando viaja para evitar este tipo de problema em Cybercafés da América Latina)

No Internet Explorer 7 você pode eliminar a maior parte dos problemas acessando Ferramentas->Opções da Internet e na aba "Avançado" clicar no botão "Restaurar padrões".

Certifique-se de limpar o cache. No Internet Explorer 6 acesse Ferramentas->Opções da Internet->Apagar Arquivos. No Firefox, Ferramentas->Limpar Histórico Recente, então clique em "detalhes", marque "cache" e clique em "limpar agora".

Todo programa que bloqueie cookies causará esse problema. Por exemplo, o CA Security Center pode bloquear cookies de qualquer programa. Tente desabilitar temporariamente seu firewall e anti-vírus para ver se não são eles que causam o problema.

Se não conseguir resolver o problema, peça ajuda. Envie uma mensagem usando o formulário de contato.',
  'user_mail_password_reset_subject' => '[WarmShowers.org] Link de recuperação de senha para [user:name]',
  'user_mail_register_admin_created_body' => '[user:name],

A site administrator at !site has created an account for you. You may now log in to !login_uri using the following username and password:

username: [user:name]

You may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you will be redirected to [user:edit-url] so you can change your password.


--  !site team',
  'user_mail_register_admin_created_subject' => 'An administrator created an account for you at  !site',
  'user_mail_register_no_approval_required_body' => '[user:name],

Obrigado por se inscrever no WarmShowers.org.

Por favor <a href="[user:one-time-login-url]">clique aqui</a> para validar seu endereço de e-mail e habilitar sua conta.

[user:one-time-login-url]

Depois de validar sua conta você poderá efetuar login em !login_uri usando o seguinte nome de usuário e senha:

Nome de usuário: [user:name]

Se tiver qualquer pergunta, responda este e-mail e faremos o melhor para lhe ajudar.

Obrigado,

Warmshowers.org administração

----
Admin (edit): <a href="[user:edit-url]">[user:edit-url]</a>',
  'user_mail_register_no_approval_required_subject' => 'Valide sua conta no WarmShowers.org',
  'user_mail_register_pending_approval_body' => 'Benvindo [user:name],

*** Você deve responder este e-mail para ter acesso ao site! ***
*** Responda escrevendo algo sobre si mesmo! ***
*** Você deve deixar o assunto do e-mail e o resto da mensagem como eles estão. ***

( Quando você responder, precisamos que diga "olá". Você não está respondendo para uma máquina e sim para a equipe do site! )

*** Você não conseguirá efetuar login até que habilitemos sua conta
*** o que normalmente acontece algumas horas depois de você mandar a resposta.

Agradecemos sua inscrição no www.WarmShowers.org.

Obrigado por se inscrever para uma conta, e por sua hospitalidade com cicloviajantes.

(Menores de idade são podem usar o serviço desse site. Se você é menor de 18 anos, por favor não continue.)

Habilitaremos sua conta após receber sua resposta.Seu nome de usuário e senha (ativos depois da habilitação) estão abaixo.

Randy Fay
Webmaster do WarmShowers.org

DEPOIS que sua conta for autorizada, efetue login no site com estas informações
nome de usuário: [user:name]

DEPOIS que sua conta for aprovada, você também poderá efetuar login clicando no seguinte link, ou copiando e colando o mesmo no navegador:

[user:one-time-login-url]

Este é um link de uso único.

Depois de efetuar login, você sempre poderá modificar sua senha e outros detalhes de conta em  [user:edit-url]

Não se esqueça de depois de efetuar login, ir à seção "minha conta", na aba "localizar no mapa" e certifique-se que sua localização está mostrada corretamente no mapa. Se tiver problemas, na própria página há instruções de como fazer isso.

Obrigado por se inscrever!

-Randy Fay e a equipe de inscrição
WarmShowers.org

--
Somente para Admin:
[user:edit-url]',
  'user_mail_register_pending_approval_subject' => 'Informação de conta para  [user:name] no WarmShowers.org - você deve responder e dizer algo sobre si mesmo',
  'user_mail_status_activated_body' => 'Benvindo [user:name],

Sua conta no www.WarmShowers.org foi ativada.

Efetue login com o seu nome de usuário e senha, você pode entrar clicando neste link ou copiando ecolando no seu navegador:

[user:one-time-login-url]

Este link só pode ser usado uma única vez. Depois de efetuar login, você será redirecionado para [user:edit-url] , assim você pode modificar sua senha caso queira.

Alguma pergunta? Basta responder esta mensagem.

Grato,
-Randy Fay
webmaster do WarmShowers.org

--
Somente para Admin:
!uri',
  'user_mail_status_activated_subject' => 'WarmShowers.org: Usuário [user:name] foi aprovado',
  'user_mail_status_blocked_body' => 'Hola [user:name]:

Sua conta no www.WarmShowers.org foi bloqueada.

Para maiores informações, por favor responda a este e-mail.

Grato.
Randy Fay -Webmaster do WarmShowers.org

Somente para Admin:
[user:edit-url]
',
  'user_mail_status_blocked_subject' => 'WarmShowers.org: a conta [user:name] foi bloqueada',
  'user_mail_status_deleted_body' => 'Olá  [user:name],

Sua conta no WarmShowers.org foi excluída, provavelmente por sua solicitação ou porque sua inscrição parecer ser inválida ou Spam.

Se tiver qualquer dúvida, responda este e-mail.

Gratos,
Randy Fay - webmaster WarmShowers.org',
  'user_mail_status_deleted_subject' => 'WarmShowers.org: conta [user:name] excluída',
  'user_picture_guidelines' => 'Queremos uma foto com você, seja em sua casa ou pedalando, por exemplo. Carregar a foto é totalmente opcional mas pode ser divertido e dá uma ideia de quem você para os membros que te contatam. Além disso, as fotos de todos os membros aparecem aleatoriamente na página inicial. Se você enviar uma nova foto e ela não aparecer imediatamente, dê o comando de recarregar completamente a página no navegador (Aperte &lt;ctrl&gt;+&lt;F5&gt;)
',
  'user_registration_help' => '<h3>Bem vindo! Por favor leia antes de continuar a inscrição</h3>
Estamos felizes Nós estamos contentes que você quer  participar dos Chuveiros Quentes (Warm Showers)! Dentro de instantes você entrará com toda a sua informação, então lhe enviaremos um e-mail de validação - você deve abrir um link que estará neste e-mail. Hoje em dia, com milhões de filtros de Spam, pode ser que você não receba o e-mail caso não nos adicione aos seus contatos, e dessa forma sua conta não será habilitada. Então...
<ol>
  <li>Agora mesmo, antes de completar o formulário, adicione o endereço <em>wsl@warmshowers.org</em> aos contatos ou &quot;lista branca&quot; de seu e-mail. Se você não nos adicionar, poderá acabar não recebendo o e-mail de validação com a sua senha, e assim não efetivará sua inscrição.</li>
  <li>Leia <a href="http://pt.warmshowers.org/content/regras" target="_blank">as Regras</a> de uso do site e lembre-se de checá-las sempre que estiver em dúvida.</li>
  <li>Ao se inscrever, lembre-se que isso significa que você pretende oferecer hospitalidade, pelo menos algum dia. Este é um site de hospitalidade recíproca. Se você está em uma cicloviagem neste momento ou por qualquer outro motivo não puder hospedar, basta selecionar a opção &quot;Não disponível atualmente&quot; no cadastro e no futuro poderá desmarcá-la em sua conta</li>
  <li>Menores (pessoas abaixo da idade legal) não podem usar o site, portanto não continue se você for menor de idade.</li>
  <li>Lembre-se que temos uma página de <a href="http://pt.warmshowers.org/faq" target="_blank">Perguntas Frequentes (FAQ)</a>, que pode tirar suas possíveis dúvidas. Lembre-se de consultá-la tenha problemas ao efetuar login no site.</li>
</ol>
<p>Pronto! Por favor, preencha o formulário abaixo com o máximo de detalhes, com atenção especial aos campos obrigatórios (marcados com <span class="form-required" title="Este campo é obrigatório.">*</span>)</p>',
  'wsuser_become_available_notification_body' => 'Olá [user:fullname] - <br/><br/>A sua conta do Warmshowers.org cujo utilizador/usuário é, [user], foi automaticamente atualizada para "disponível para hospedar" porque a data prevista de retorno que especificou já passou. Se continua indisponível para hospedar ligue-se, pff, a Warmshowers.org e atualize o seu perfil. Pode ligar-se ao site visitando-o ou <a href=\'[user:login_url]\'>clicando neste link</a>. Obrigado!<br/><br/>-Randy Fay<br/>Warmshowers.org Webmaster<br/>---<br/><a href=\'[user:url]\'>Profile link</a><br/><a href=\'[account-edit-url]\'>Edit profile link</a><br/><a href=\'[user:login_url]\'>Direct login link</a><br/>',
  'wsuser_become_available_notification_subject' => 'Agora está listado como "disponível para hospedar" em Warmshowers.org ([user:fullname])',
  'wsuser_notavailable_reminder_body' => 'Olá [user:fullname], <br/><br/>A sua conta warmshowers.org cujo utilizador/usuário é, [user], está marcado como "não disponível para hospedar". Agradecíamos que se ligasse ao site e no perfil definisse a data que prevê voltar a ter disponibilidade (data de retorno). Se não souber quando vai voltar a ter disponibilidade, coloque uma data longínqua do tipo 2025-01-01. Pode alterá-la sempre que quiser. Ligue-se ao site com o seu utilizador/usuário e palavra passe em  <a href=\'https://pt.warmshowers.org\'>pt.warmshowers.org</a> ou <a href=\'[user:login_url]\'>clique neste link para se ligar automaticamente</a><br/><br/>-Randy Fay<br/>Warmshowers.org Webmaster<br/>---<br/><a href=\'[user:url]\'>Profile link</a><br/><a href=\'[account-edit-url]\'>Edit profile link</a><br/><a href=\'[user:login_url]\'>Direct login link</a><br/>',
  'wsuser_notavailable_reminder_subject' => 'O seu perfil está marcado como "não disponível para hospedar". Atualize, pff a data prevista de retorno. ([user:fullname])',
);
  $realm_variables['language']['ro'] = array(
  'wsuser_become_available_notification_body' => 'Hi [user:fullname] - <br/><br/>Your Warmshowers.org account, [user], has been automatically updated to show you as "available to host" because the date that you specified you would become available after has passed. If you are still unavailable to host please log into Warmshowers.org and update your account. You can log in by visiting the site or just <a href=\'[user:login_url]\'>clicking this link</a>. Thanks!<br/><br/>-Randy Fay<br/>Warmshowers.org Webmaster<br/>---<br/><a href=\'[user:url]\'>Profile link</a><br/><a href=\'[account-edit-url]\'>Edit profile link</a><br/><a href=\'[user:login_url]\'>Direct login link</a><br/>',
  'wsuser_become_available_notification_subject' => 'You are now listed as available to host on Warmshowers.org ([user:fullname])',
  'wsuser_notavailable_reminder_body' => 'Hi [user:fullname], <br/><br/>Your warmshowers.org account, [user], is marked as unavailable to host. Could you please log in and set the date that you expect to return? If you don\'t know when or if you\'ll be able to host, just put in a date like 2025-01-01. You can change it at any time. You can log in with your username and password at <a href=\'https://ro.warmshowers.org\'>ro.warmshowers.org</a> or <a href=\'[user:login_url]\'>click this link to log in automatically</a><br/><br/>-Randy Fay<br/>Warmshowers.org Webmaster<br/>---<br/><a href=\'[user:url]\'>Profile link</a><br/><a href=\'[account-edit-url]\'>Edit profile link</a><br/><a href=\'[user:login_url]\'>Direct login link</a><br/>',
  'wsuser_notavailable_reminder_subject' => 'Your Warmshowers account is marked unavailable. Please update your return date. ([user:fullname])',
);
  $realm_variables['language']['ru'] = array(
  'comment_notify_default_mailtext' => 'Здравствуйте, [comment-subscribed:author:fullname],

[comment:author:fullname] оставил(а) комментарий к записи: "[comment:node:title]":

----
[comment:title]
[comment:body]
----

Вы можете посмотреть комментарий на сайте и ответить на него <a href="[comment:url]">по этой ссылке</a>. (Пожалуйста, не отвечайте на это письмо, поскольку оно придет только к администрации сайта.)

Вы можете отключить функцию информирования о комментариях к этой теме, нажав <a href="[comment-subscribed:unsubscribe-url]">здесь</a>.

- Команда Warmshowers.org',
  'contact_form_information' => '<p>Вы можете связаться с нами по электронной почте.</p>

<p>Если вам нужна помощь при пользовании данным сайтом, пожалуйста прочитайте вначале ответы на <a href="/faq">Частые Вопросы</a>, а также раздел <a href="http://www.warmshowers.org/forums/website-help-and-support">Помощь на Форуме</a>. Если вы не нашли ответ в данных разделах, оставьте свой вопрос в разделе Помощь на Форуме; вам обязательно помогут, и ответ на ваш вопрос поможет другим участникам.</p>

<p>If you speak English it will help the admins a *lot* if you can ask your question in English. Thanks! </p>',
  'inactive_user_block_notify_text' => 'Hello %username,

  This automatic message is to notify you that your account on %sitename has been automatically disabled due to no activity for more than %period.

  Please visit us at %siteurl to have your account re-enabled.

Sincerely,
  %sitename team',
  'inactive_user_block_warn_text' => 'Hello %username,

  We haven\'t seen you at %sitename since %lastaccess, and we miss you!  This automatic message is to warn you that your account will be disabled in %period unless you come back and visit us before that time.

  Please visit us at %siteurl.

Sincerely,
  %sitename team',
  'inactive_user_delete_notify_text' => 'Hello %username,

  This automatic message is to notify you that your Warmshowers.org account has been automatically removed because you didn\'t log in for more than %period.

  Please visit us at %siteurl if you would like to create a new account.

Sincerely,
-Randy Fay
Warmshowers.org Webmaster',
  'inactive_user_delete_warn_text' => 'Hello %username,

Your account at Warmshowers.org will soon be removed (in %period) unless you log in to let us know you still want it. Please log into the site at http://www.warmshowers.org and update your account.  If you don\'t know your username or password that\'s OK - you can log in with your email address. If you don\'t know your password, please visit http://www.warmshowers.org/user/password to get a one-time login link emailed to you.

You haven\'t logged in %lastaccess.   This automatic message is to warn you that your account will be completely removed in %period unless you log into the site before that time.

  Please visit us at %siteurl.

Sincerely,
-Randy Fay
Warmshowers.org webmaster',
  'inactive_user_notify_text' => 'Здравствуйте, %username,

Вы не посещали сайт Warmshowers.org с %lastaccess, и мы скучаем по вас! Мы просим всех заходить на сайте хотя бы раз в год, чтобы удостовериться, что ваша информация корректна. Мы были бы рады, если вы зайдете на сайт и обновите информацию о себе. Или хотя бы просто зайдете на сайт, чтобы система знала, что вы по крайней мере получаете письма на этот адрес.

Пожалуйста, войдите в ближайшее время на %siteurl.  Если вы не входите на сайт в течение календарного года, ваш профиль будет удален (перед этим вы получите еще одно письмо с предупреждением).

Если вы забыли свой пароль просто перейдите по ссылке http://www.warmshowers.org/user/password и введите свою электронную почту (%useremail) - вы получите одноразовую ссылку входа.

Искренне ваш,
-Рэнди Фэй
Вебмастер Warmshowers.org

------
Admin Information
Search: http://www.warmshowers.org/search/wsuser/%useremail
Username: %username
Email: %useremail',
  'node_notify_default_mailtext' => 'Здравствуйте, [comment:node:author:fullname],

Вы получили комментарий к записи: "[comment:node:title]":

----
[comment:title]
[comment:body]
----

Вы можете посмотреть комментарий на сайте, пройдя по <a href="[comment:url]">ссылке</a>.  (Пожалуйста, не отвечайте на это письмо, поскольку оно придет только к администрации сайта.)

Вы будете получать письма информирующие вас о всех комментариях к вашим записям..

- Команда Warmshowers',
  'user_mail_password_reset_body' => 'Здравствуйте, [user:name],

Вы (или кто-то от вашего имени) запросил одноразовую ссылку для входа в ваш профиль на warmshowers.org. Если это не были вы, просто проигнорируйте данное сообщение.

Если это сделали вы, то, пожалуйста, <a href="[user:one-time-login-url]">нажмите здесь для входа и изменения вашего пароля </a>. Если вы не можете перейти по ссылке, пожалуйста, скопируйте и вставьте  в адресную строку вашего браузера следующее:

[user:one-time-login-url]

Вход по этой ссылке может быть осуществлен один раз в течение суток после отправления. Если вы не использовали данную ссылку, ничего не произойдет.

После входа вы будете перенаправлены на страницу <a href="[user:edit-url]">редактирования профиля</a>, где вы сможете изменить пароль.

Спасибо,
-Рэнди Фэй и команда регистрации
WarmShowers.org

Возможные проблемы:

Если вы видите следующее "Sorry - you don\'t have access to that page (error 403)"/«Извините, у вас нет доступа к этой странице (ошибка 403)», это значит, что в вашем браузере отключены кукисы. Чтобы правильно их настроить, пожалуйста, прочитайте эту <a href="http://www.warmshowers.org/content/i-try-log-it-still-says-im-not-logged">статью в разделе Частые Вопросы</a>.

Если вы не можете решить проблему, ответьте на письмо с просьбой о помощи.
',
  'user_mail_password_reset_subject' => '[Warmshowers.org] Одноразовая ссылка входа для  [user:name]',
  'user_mail_register_admin_created_body' => '[user:name],

A site administrator at !site has created an account for you. You may now log in to !login_uri using the following username and password:

username: [user:name]

You may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you will be redirected to [user:edit-url] so you can change your password.


--  !site team',
  'user_mail_register_admin_created_subject' => 'An administrator created an account for you at  !site',
  'user_mail_register_no_approval_required_body' => '[user:name],

Спасибо за регистрацию на сайте Warmshowers.org.

Пожалуйста, перейдите по <a href="[user:one-time-login-url]">ссылке</a> для подтверждения и активации вашего профиля. Если вы не можете перейти по ссылке, пожалуйста, скопируйте и вставьте  в адресную строку вашего браузера следующее:

[user:one-time-login-url]

После активации вашего профиля по вышеприведенной ссылке вы сможете войти !login_uri используя ваше имя пользователя и пароль:

Имя пользователя: [user:name]
Пароль: The password you entered

Если у вас возникли какие-то вопросы, вы можете ответить на это письмо и мы с радостью поможем вам.

Спасибо,


-- команда !site

--Admin use only
Admin (edit): <a href="[user:edit-url]">[user:edit-url]</a>',
  'user_mail_register_no_approval_required_subject' => '[Warmshowers.org] Подтвердите регистрацию на Warmshowers.org',
  'user_mail_register_pending_approval_body' => 'Welcome [user:name],

*** You must reply to this email to get access to the site! ***
*** Please reply saying something about yourself! ***
*** You must leave the message\'s subject and the rest of this message as they are. ***

( And when you reply - we need you to say "hi". You\'re not replying to a machine but to Randy and the rest of the team! )

*** You won\'t be able to log in until we enable your account
*** which is usually at least several hours after you send your reply.

Thank you for your registration at www.warmshowers.org.

Thank you for applying for an account, and for your hospitality to touring cyclists.

You need to acknowledge that you\'ve received this message by replying to it and saying something about yourself so we can correspond with you and try to make sure everybody on the list is a bona fide member.

I will enable your account after I receive your response.

(Minors - people not of legal age - are not allowed to use the site. So if you are under age, please do not continue.)

I\'ll be happy to approve your account. Your username and password (active after we correspond) are below.

Randy Fay
Warmshowers.org webmaster

AFTER your account has been authorized (after I receive your response and
activate your account), your username and password details will be:

username: [user:name]

AFTER your account is approved, you may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you can always change your password and other account details at  [user:edit-url]

Also: After you log in, please go to "my account" and the "map my location" tab and make sure your location shows correctly on the map. There are instructions on that page that will help you set it.

Thanks for signing up!

-Randy Fay and the registration team
Warmshowers.org

--
Administrator use only:
[user:edit-url]',
  'user_mail_register_pending_approval_subject' => 'Account info for  [user:name] at WarmShowers.org-You must reply and say something about yourself',
  'user_mail_status_activated_body' => 'Добро пожаловать, [user:name],

Ваш профиль на сайте www.warmshowers.org активирован.

Вы можете войти на сайт, используя имя пользователя и пароль или же пройдя по этой <a href="[user:one-time-login-url]">ссылке</a>.

Обратите внимание, что это одноразовая ссылка.

После входа на сайт, вы будете перенаправлены на страницу <a href="[user:edit-url]">редактирования вашего профиля,</a> где вы сможете при желании изменить ваш пароль.

Если у вас есть вопросы, можете ответить на это письмо.

Спасибо,
-Рэнди Фэй
Вебмастер Warmshowers.org

--
Admin use only:
!uri',
  'user_mail_status_activated_subject' => '[Warmshowers.org] Ваш профиль активирован',
  'user_mail_status_blocked_body' => 'Hello [user:name],

Your account on www.warmshowers.org has been blocked.

If you have any questions, please reply to this email.

Thanks,
Randy Fay
Warmshowers.org webmaster

Admin use:
[user:edit-url]',
  'user_mail_status_blocked_subject' => 'Warmshowers.org: [user:name] account blocked',
  'user_mail_status_deleted_body' => 'Здравствуйте, [user:name],

Ваш профиль на сайте www.warmshowers.org удален, скорее всего, по вашему запросу или из-за того, что вы не заходили на сайт долгое время.

Если у вас есть какие-то вопросы, пожалуйста, ответьте на это письмо.

Спасибо,
Рэнди Фэй
Warmshowers.org',
  'user_mail_status_deleted_subject' => '[Warmshowers.org] профиль [user:name] удален',
  'user_picture_guidelines' => 'Мы были бы рады видеть <i>ваше</i> фото. Было бы хорошо - вместе с велосипедом, в ваших краях. Это целиком на ваше усмотрение, но было бы приятно – фотографии пользователей в случайном порядке показываются на стартовой странице. <i>Если вы загружаете новую фотографию и она не отображается, сделайте принудительное обновление страницы (зажмите клавишу SHIFT и нажмите кнопку ОБНОВИТЬ в вашем браузере).</i>',
  'user_registration_help' => '<h3>Добро пожаловать! Пожалуйста, прочитайте вначале этот текст </h3>
Мы рады, что вы хотите стать участником Warm Showers! Вам необходимо ввести информацию о себе, и далее мы пришлем вам письмо-подтверждение регистрации. Пожалуйста, перейдите по ссылке, которая будет указана в письме. Но, обратите внимание – в наши дни многие письма блокируются спам-фильтрами, и вы можете не увидеть письмо-подтверждение в вашем почтовом ящике, если не добавите заранее наш адрес в «белый список». В этом случае ваша регистрация не будет завершена. Поэтому…

<ol><li>
Прямо сейчас, до заполнения формы регистрации, пожалуйста, добавьте адрес wsl@warmshowers.org в «белый список» вашего почтового ящика. Инструкцию, как это сделать во многих почтовых службах, вы можете найти <a href="/spam_filters" target="_blank">здесь</a>. Если вы этого не сделаете, возможно вы не получите письмо-подтверждение регистрации с вашим паролем и не сможете зарегистрироваться. </li>
<li>Пожалуйста, прочитайте <a href="/the_rules" target="_blank">Правила</a> и не забудьте обращаться к ним, если у вас есть какие-то сомнения.</li>
<li>Если вы регистрируетесь, пожалуйста, не забывайте о том, что это подразумевает предоставление ночлега для велотуристов с данного сайта, по крайней мере иногда. Этот сайт создан для взаимного гостеприимства велотуристов. (Если вы в путешествии или по каким-то иным соображениям в данный момент не можете предоставить ночлег в данный момент, поставьте статус «в данный момент недоступен»; в дальнейшем вы можете изменить этот статус в своем профиле.)</li>
<li>Несовершеннолетние не имеют права регистрироваться на данном сайте, поэтому если вы относитесь к этой категории, пожалуйста, не делайте этого. </li>
<li>Обратите внимание на раздел <a href="/faq" target="_blank">Частые Вопросы</a>, который поможет решить проблемы при их наличии. Также, обратитесь к этому разделу, если у вас проблемы с входом на сайт. </li>
<li>Беспокоит ли вас то, что информация, которую вы вводите здесь будет доступна всем? Сайт создан таким образом, что информация вашего профиля невидима для «внешнего мира»; только пользователи сайта могут видеть вашу информацию после того, как они вошли в систему. Ваш электронный адрес всегда скрыт, даже от других пользователей. </li>
<li>Ваша личная безопасность очень важна для нас. Пожалуйста, прочитайте раздел <a href="/personal-security">Личная Безопасность</a> для получения более подробной информации.</li>
<li>Наша <a target="_blank" href="/privacy">Политика Конфиденциальности</a> подтверждает, что ваш профиль доступен только для пользователей сайта и только пользователи могут вступать в контакт с вами.</li>
<li>При регистрации вы соглашаетесь с <a target="_blank" href="/terms">Условиями Пользования</a>,которые подразумевают, что <strong>вы и только вы несете ответственность за любое общение и взаимодействие с другими пользователями Warmshowers</strong></li>
</ol>
<br/><br/>
Вот и все! Пожалуйста, перейдите к нижеследующему разделу для заполнения формы регистрации.
<hr/>',
  'wsuser_become_available_notification_body' => 'Здравствуйте, [user:fullname] - <br/><br/>Ваша учетная запись на сайте Warmshowers.org, [user], автоматически переведена в состояние возможности приема гостей, потому что наступила дата, которую вы указывали как дата вашей доступности. Если вы все еще не можете принимать гостей, пожалуйста, войдите на сайт Warmshowers.org под своей учетной записью и обновите информацию. Вы можете войти на сайт по <a href=\'[user:login_url]\'>этой ссылке</a>. Спасибо!<br/><br/>-Рэнди Фэй<br/>Вебмастер Warmshowers.org<br/>---<br/><a href=\'[user:url]\'>Profile link</a><br/><a href=\'[account-edit-url]\'>Edit profile link</a><br/><a href=\'[user:login_url]\'>Direct login link</a><br/>',
  'wsuser_become_available_notification_subject' => 'Теперь вы можете принять гостей с Warmshowers.org ([user:fullname])',
  'wsuser_notavailable_reminder_body' => 'Здравствуйте, [user:fullname], <br/><br/>Ваша учетная запись на сайте Warmshowers.org, [user], сейчас имеет статус "Не могу принять гостей". Пожалуйста, войдите на сайт и установите дату вашего возвращения. Если вы не знаете когда у вас будет возможность принимать гостей, просто установите дату в будущем, например 2025-01-01. Вы можете изменить ее в любой момент, просто войдите на сайт под своей учетной записью <a href=\'https://ru.warmshowers.org\'>ru.warmshowers.org</a> или <a href=\'[user:login_url]\'>нажмите здесь для перехода на сайт</a><br/><br/>-Рэнди Фэй<br/>Вебмастер Warmshowers.org <br/>---<br/><a href=\'[user:url]\'>Profile link</a><br/><a href=\'[account-edit-url]\'>Edit profile link</a><br/><a href=\'[user:login_url]\'>Direct login link</a><br/>',
  'wsuser_notavailable_reminder_subject' => 'Ваша учетная запись на Warmshowers.org сейчас в режиме "Недоступен". Пожалуйста, обновите дату возвращения. ([user:fullname])',
);
  $realm_variables['language']['sr'] = array(
  'comment_notify_default_mailtext' => 'Здраво [comment-subscribed:author:fullname],

[comment:author:fullname] је оставио коментар на: "[comment:node:title]":

----
[comment:title]
[comment:body]
----

Можете прегледати и одговорити на коментар тако што <a href="[comment:url]"> пратите ову везу</a>. (Молимо Вас да не одговарате на електронску поруку, јер порука одлази само уредницима интернет странице.)

Можете да искључите опцију да добијате електронске поруке када неко одговри на ову поруку тако што ће те пратити  <a href="[comment-subscribed:unsubscribe-url]">ову везу</a>.

- Warmshowers.org тим',
  'contact_form_information' => '<p>Слободно нас контактирајте путем електронске поште.</p>

<p>Уколико Вам је потребна било каква помоћ око употребе интернет странице, молимо Вас да најпре прочитате <a href="/faq">најчешћа питања</a>, затим потражите помоћ на <a href="http://www.warmshowers.org/forums/website-help-and-support">Форуму </a> и слањем питања тамо, јер тамо Вам и други могу помоћи, а сви одговори помажу осталима који пратеа форум такође.</p>

<p>If you speak English it will help the admins a *lot* if you can ask your question in English. Thanks! </p>',
  'inactive_user_block_notify_text' => 'Hello %username,

  This automatic message is to notify you that your account on %sitename has been automatically disabled due to no activity for more than %period.

  Please visit us at %siteurl to have your account re-enabled.

Sincerely,
  %sitename team',
  'inactive_user_block_warn_text' => 'Hello %username,

  We haven\'t seen you at %sitename since %lastaccess, and we miss you!  This automatic message is to warn you that your account will be disabled in %period unless you come back and visit us before that time.

  Please visit us at %siteurl.

Sincerely,
  %sitename team',
  'inactive_user_delete_notify_text' => 'Hello %username,

  This automatic message is to notify you that your Warmshowers.org account has been automatically removed because you didn\'t log in for more than %period.

  Please visit us at %siteurl if you would like to create a new account.

Sincerely,
-Randy Fay
Warmshowers.org Webmaster',
  'inactive_user_delete_warn_text' => 'Hello %username,

Your account at Warmshowers.org will soon be removed (in %period) unless you log in to let us know you still want it. Please log into the site at http://www.warmshowers.org and update your account.  If you don\'t know your username or password that\'s OK - you can log in with your email address. If you don\'t know your password, please visit http://www.warmshowers.org/user/password to get a one-time login link emailed to you.

You haven\'t logged in %lastaccess.   This automatic message is to warn you that your account will be completely removed in %period unless you log into the site before that time.

  Please visit us at %siteurl.

Sincerely,
-Randy Fay
Warmshowers.org webmaster',
  'inactive_user_notify_text' => 'Здраво %username,

Нисте се пријавили на интернет страницу Warmshowers.org од %lastaccess, и недостајали сте нам! Од сваког члана тражимо да се барем једном у току године, да провери податке о себи и потврди њихову тачност, те бисмо ценили ако бисте се пријавили на интернет страницу, и освежили податке о себи уколико је то потребно. У најмању руку молимо вас да се барем просто пријавите на интернет страницу, да бисмо знали да још увек примате поруке путем ове електронске адресе.

Молимо Вас да освежите своје личне податке на  %siteurl. Уколико се не пријавите нити један пут токо календарске године на нашу интернет страницу, на крају ће ваш налог бити трајно уклоњен (наравно примићете више од једне електронске поруке које ће вас упозорити на брисање налога).

Уколик сте заборавили Вашу лозинку, довољно је да погледате  http://www.warmshowers.org/user/password и да унесете вашу електронску адресу (%useremail) и добићете електронску поруку са једнократном везом помоћу које ће те моћи да се пријавите на интернет страницу.

Хвала Вам
-Randy Fay
Уредник интернет стрнице Warmshowers.orг

------
Информације за администраторе
Претражите: http://www.warmshowers.org/search/wsuser/%useremail
Корисничко име: %username
Електронска адреса: %useremail
One-time login: [login-one-time]',
  'node_notify_default_mailtext' => 'Здраво [comment:node:author:fullname],

Примили сте коментар на: "[comment:node:title]":

----
[comment:title]
[comment:body]
----

Можете видети коментар пратећи <a href="[comment:url]">ову везу</a>.  (Молимо Вас да не одговарате на електронску поруку, јер порука одлази само уредницима интернет странице.)

Добијаћете овакве електронске поруке за све одговоре на вашу поруку на форуму.

- Warmshowers тим',
  'user_mail_password_reset_body' => 'Здраво [user:name],

Ви (или неко у ваше име) је затражио једнократну везу за пријаву на ваш налог на интернет страници warmshowers.org. Уколико то нисте били ви, слободно занемарите ову поруку.

Молимо вас  <a href="[user:one-time-login-url]">пратите ову везу да бисте се пријавили и променили башу лозинку</a>. Уколико нисте у могућности да пратите везу, препишите вез у и унесите сами директно у ваш интернет претраживач:

[user:one-time-login-url]

Ово је једнократна веза за пријављивање, дакле она се може користити само једно. Њено трајање истиче за један дан. Након једног дана престаје важност ове везе, али нема никаквих последица уколико не искористите ову везу.

Након пријављивања, бићете аутоматски преусмерени на <a href="[user:edit-url]"> страницу за измену ваше личне странице</a> где ћете моћи да измените вашу лозинку.

Хвала,
-Randy Fay и регистрациони тим
WarmShowers.org


Решавање проблема:

Уколико добијете поруку "Sorry - you don\'t have access to that page (error 403)", можете бити готово сигурни да вам је у интернет претраживачу искључена опција коришћења тзв.  cookies .  Да бисте ову опцију правилно наместили молимо вас да погледате <a href="http://www.warmshowers.org/content/i-try-log-it-still-says-im-not-logged"> овај текст на ту туме у оквиру најчешће постављених питања</a>.

Уколико нисте у могућности да разрешите овај проблем, молимо Вас да затражите помоћ. Тако што ће те одговорити на ову електронску поруку и затражити помоћ.',
  'user_mail_password_reset_subject' => '[Warmshowers.org] Једнократна веза за пријављивање на интернет страницу за корисника [user:name]',
  'user_mail_register_admin_created_body' => '[user:name],

A site administrator at !site has created an account for you. You may now log in to !login_uri using the following username and password:

username: [user:name]

You may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you will be redirected to [user:edit-url] so you can change your password.


--  !site team',
  'user_mail_register_admin_created_subject' => 'An administrator created an account for you at  !site',
  'user_mail_register_no_approval_required_body' => '[user:name],

Хвала Вам што сте се пријавили на Warmshowers.org.

Молим вас идите не <a href="[user:one-time-login-url]">овде</a>  како бисте потврдили вашу електронску адреси и како бисте покренули ваш налог. (Уколико нисте у могућности да продужите пратећи ту везу, молим да је препишете и упишете у ваш претраживач:

[user:one-time-login-url]

Након што потврдите користећи везу изнад, бићете у могућности да се пријавите на нашу интернете страницу !login_uri користећи корисничко име и лозинку које можете видети у наставку:

username: [user:name]
password: The password you entered

Уколико имате нека питања, можете одговорити на ову електронску поруку, и ми жемо

--  !site team
--Само за администраторе
Admin (edit): <a href="[user:edit-url]">[user:edit-url]</a>',
  'user_mail_register_no_approval_required_subject' => '[Warmshowers.org] Потврдите ваш налог на Warmshowers.org',
  'user_mail_register_pending_approval_body' => 'Welcome [user:name],

*** You must reply to this email to get access to the site! ***
*** Please reply saying something about yourself! ***
*** You must leave the message\'s subject and the rest of this message as they are. ***

( And when you reply - we need you to say "hi". You\'re not replying to a machine but to Randy and the rest of the team! )

*** You won\'t be able to log in until we enable your account
*** which is usually at least several hours after you send your reply.

Thank you for your registration at www.warmshowers.org.

Thank you for applying for an account, and for your hospitality to touring cyclists.

You need to acknowledge that you\'ve received this message by replying to it and saying something about yourself so we can correspond with you and try to make sure everybody on the list is a bona fide member.

I will enable your account after I receive your response.

(Minors - people not of legal age - are not allowed to use the site. So if you are under age, please do not continue.)

I\'ll be happy to approve your account. Your username and password (active after we correspond) are below.

Randy Fay
Warmshowers.org webmaster

AFTER your account has been authorized (after I receive your response and
activate your account), your username and password details will be:

username: [user:name]

AFTER your account is approved, you may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you can always change your password and other account details at  [user:edit-url]

Also: After you log in, please go to "my account" and the "map my location" tab and make sure your location shows correctly on the map. There are instructions on that page that will help you set it.

Thanks for signing up!

-Randy Fay and the registration team
Warmshowers.org

--
Administrator use only:
[user:edit-url]',
  'user_mail_register_pending_approval_subject' => 'Account info for  [user:name] at WarmShowers.org-You must reply and say something about yourself',
  'user_mail_status_activated_body' => 'Добродошли [user:name],

Ваш налог на интернет страници www.warmshowers.org је управо активиран.
Можете се пријавити на интернет страницу коритећи ваше корисничко име и лозинку коју већ поседујете, или се можете пријавити тако што <a href="[user:one-time-login-url]">испратити ову везу</a>.

Ово је једнократно пријављивање на интернет страницу, и могуће је користити само једном.
Након пријављивања, бићете аутоматски преусмерени на <a href="[user:edit-url]"> страницу за измену ваше личне странице</a> где ћете моћи да измените вашу лозинку уколико желите.

Имате било каква питања? Довољно је да одговорите на ову електронску поруку.

Хвала Вам
-Randy Fay
Уредник интернет стрнице Warmshowers.org

--Само за администраторе

!uri',
  'user_mail_status_activated_subject' => '[Warmshowers.org] Ваш налог је направљен и потврђен',
  'user_mail_status_blocked_body' => 'Hello [user:name],

Your account on www.warmshowers.org has been blocked.

If you have any questions, please reply to this email.

Thanks,
Randy Fay
Warmshowers.org webmaster

Admin use:
[user:edit-url]',
  'user_mail_status_blocked_subject' => 'Warmshowers.org: [user:name] account blocked',
  'user_mail_status_deleted_body' => 'Здраво [user:name],

Ваш налог на интернет страници www.warmshowers.org је обрисан, највероватније по Вашем захтеву или као последица тога што је постао неважећи, или је кориштен за дистрибуцију штетних порука

Имате било каква питања? Довољно је да одговорите на ову електронску поруку.
Хвала,
Randy Fay
Warmshowers.org',
  'user_mail_status_deleted_subject' => '[Warmshowers.org] [user:name] налог је обрисан',
  'user_picture_guidelines' => 'Волели бисмо фотографију на којој сте <i>Ви</i>, најбоље како се возите бициклом у вашој околини. Ово је потпуно добровољно наравно, али може бити забавно. Случајно одабране фотографије се показују на насловној страни наше интернет странице такође. <i>Уколико поставите вашу фотографију и не приказује се одма на начин како треба, потребно је да освежите ваш интернет претраживач. (Држећи тастер SHIFT и притисните REFRESH дугме у вашем претраживачу.)</i>',
  'user_registration_help' => '<h3>Добродошли! Молим Вас да најпре прочитате ово пре него што наставите да читате било шта друго што је у наставку</h3>
Драго нам је да сте одлучили да се прикључите Warm Showers заједници! За који минут ћемо тражити од Вас да унесете све Ваше податке, након чега ћемо Вам послати електронску поруку за потврду, у којем морате да пратите упутства како би се процес регистрације правилно завршио. Наравно у данашње време, милиони нежељених електронских порука круже светском мрежом свакодневно, тако да постоји могућност да ће и наша електронска порука бити жртва неких од система заштите тако да је потребно да нашу електронску адресу означите као прихватљиву како ваш систем заштите у будућности не би одбијао наше поруке, уколико то не учините постаји могућност да ваш налог на нашој интернет страници остане заувек неактиван, дакле...
<ol><li>Сада, пре него пошаљете овај формулар, молим Вас додајте електронску адресу wsl@warmshowers.org у вашу белу листу (листу дозвољених адреса) вашег филтра СПАМ порука, или једноставно пошаљите једну кратку електронску поруку на нашу адресу са насловом поруке "test" Уколико не учините овако, постоји могућност да не примите електронску поруку за потврду ваше електронске адресе, што за последицу може имати да нећете бити у могућности да се пријавите на систем.</li>
<li>Молим Вас да погледате <a href="/the_rules" target="_blank">Правила</a> и не заборавите  их проверите ponovo сваког пута када сте у недоумици око нечега што има везе са нашом заједницом.</li>
<li>Када се пријавите на нашу интернет страницу, молим Вас имајте у виду да то значи да намеравате да угостите неког, није битно када. Ово је интернет страница која нуди узајамну гостопримљивост. (Уколико сте тренутно на путовању, или из неког другог разлога тренутно нисте у могућности да угостите неког, молимо Вас да то наведете на вашем профилу на тај начин што ће те опцију  "not currently available" ставити да је важећа, и након тога када опет будете у могућности да угостите неког исту опцију на вашем налогу потребно је да исту опцију означите неважећом.)</li>
<li>Малолетници ма није дозвољено коришћење услуга наше интернет странице, тако да Вас молимо да то поштујете.</li>
<li>Запамтите да постоји одељак са <a href="/faq" target="_blank">Често постављена питања</a>, где је могуће наћи одговоре на ваша питања, а обавезно поревире овај одељак уколико имате порблема са пријављивањем на нашу интернет страницу.</li>

<li>Да ли сте забринути за то ко може видети податке о вама које сте оставили на нашој интернет страници? Наша страница је направљена тако да ваше информације нису видљиве за неког ко није члан наше заједнице, само чланови  након пријављивања на нашу интернет страницу имају могућност да виде податке које сте оставили о себи. Поред тога, ваша електронска адреса је сакривена чак и за чланове заједнице.</li>
<li>Ваша лична безбедност нам је такође веома важна. Моимо вас да прочитате одељак <a href="/personal-security">Лична Безбедност</a> да бисте добили још информација на ову тему.</li>
<li>Наша <a target="_blank" href="/privacy">Политика приватности</a> вас подсећа да је ваш налог и подаци у њему видљив искључиво другим члановима, и да само они могу да Вас контактирају.</li>
<li>Пријављивањем ви потврђујете  <a target="_blank" href="/terms">Услове употребе</a> који наглашавају да <strong>ви и само ви сте одговорни за сваки однос са било којим другим чланом Warmshowers заједнице</strong></li>
</ol>
<br/><br/>
У реду, то је све. Молим вас наставите даљ и попуните податке за учлањење.
<hr/>',
  'wsuser_become_available_notification_body' => 'Здраво [user:fullname] - <br/><br/>Ваш Warmshowers.org налог, [user], је аутоматски освежен и сада је означен као доступан, јер је прошао датум који сте навели као дан када ће те опет бити доступни. Уколико то није случај, и ви још увек нисте у могућности да угостите неког, молимо Вас да се пријавите на интернет страницу Warmshowers.org и да освежите податке у Вашем налогу. Можете се пријавити тако што ће те посетити интернет страницу, или само <a href=\'[user:login_url]\'>пратећи ову везу</a>. Хвала!<br/><br/>-Randy Fay<br/>Уредник Warmshowers.org интернет странице<br/>---<br/><a href=\'[user:url]\'>Веза ка налогу</a><br/><a href=\'[account-edit-url]\'>Веза ка измени налога</a><br/><a href=\'[user:login_url]\'>Непосредна веза ка пријави</a><br/>',
  'wsuser_become_available_notification_subject' => 'Сада сте означени као доступни на Warmshowers.org ([user:fullname])',
  'wsuser_notavailable_reminder_body' => 'Здраво [user:fullname], <br/><br/>Ваш warmshowers.org налог, [user], је означен као недоступан. Можете ли се пријавити на интернет страницу и подесити датум вашег повратка? Уколико не знате тачно када се враћате, или ако уопште нисте сигурни да ли ће те бити у прилици да угостите неког, довољно је да поставите неки датум 2025-01-01. То можете променити било када. Можете се пријавити користећи ваше корисничко име и лозинку <a href=\'https://rs.warmshowers.org\'>rs.warmshowers.org</a> или <a href=\'[user:login_url]\'>пратећи ову везу да бисте се аутоматски пријавити</a><br/><br/>-Randy Fay<br/>Уредник Warmshowers.org интернет странице<br/>---<br/><a href=\'[user:url]\'>Веза ка налогу</a><br/><a href=\'[account-edit-url]\'>Веза ка одељку за измену налога</a><br/><a href=\'[user:login_url]\'>Непосредна веза ка пријављивању</a><br/>',
  'wsuser_notavailable_reminder_subject' => 'Ваш Warmshowers налог је означен као неактиван. Молим вас да освежите датум вашег повратка. ([user:fullname])',
);
  $realm_variables['language']['sv'] = array(
  'comment_notify_default_mailtext' => 'Hej [comment-subscribed:author:fullname],

[comment:author:fullname] har kommenterat: "[comment:node:title]":

----
[comment:title]
[comment:body]
----

Du kan visa och svara på kommentaren genom att <a href=”[comment:url]">klicka här</a>. (Svara inte på detta mail då det endast når ansvariga för hemsidan.)

Vill du inte längre få ett mail när någon svarar på denna tråd kan du ställa in det genom att <a href=”[comment-subscribed:unsubscribe-url]">klicka här</a>.

- Warmshowers.org-gruppen',
  'contact_form_information' => '<p>Kontakta oss här via mail.</p>

<p>Om du behöver hjälp med att använda hemsidan, läs först igenom våra <a href=”/faq">Vanliga frågor</a>. Titta sedan efter ditt problem i vårt <a href=”http://www.warmshowers.org/forums/website-help-and-support”>Hjälp-forum</a> eller skapa en ny tråd för din fråga. Där kan andra hjälpa dig och svaren kan också vara till god hjälp för andra med samma bekymmer.</p>

<p>If you speak English it will help the admins a *lot* if you can ask your question in English. Thanks! </p>',
  'inactive_user_block_notify_text' => 'Hello %username,

  This automatic message is to notify you that your account on %sitename has been automatically disabled due to no activity for more than %period.

  Please visit us at %siteurl to have your account re-enabled.

Sincerely,
  %sitename team',
  'inactive_user_block_warn_text' => 'Hello %username,

  We haven\'t seen you at %sitename since %lastaccess, and we miss you!  This automatic message is to warn you that your account will be disabled in %period unless you come back and visit us before that time.

  Please visit us at %siteurl.

Sincerely,
  %sitename team',
  'inactive_user_delete_notify_text' => 'Hello %username,

  This automatic message is to notify you that your Warmshowers.org account has been automatically removed because you didn\'t log in for more than %period.

  Please visit us at %siteurl if you would like to create a new account.

Sincerely,
-Randy Fay
Warmshowers.org Webmaster',
  'inactive_user_delete_warn_text' => 'Hello %username,

Your account at Warmshowers.org will soon be removed (in %period) unless you log in to let us know you still want it. Please log into the site at http://www.warmshowers.org and update your account.  If you don\'t know your username or password that\'s OK - you can log in with your email address. If you don\'t know your password, please visit http://www.warmshowers.org/user/password to get a one-time login link emailed to you.

You haven\'t logged in %lastaccess.   This automatic message is to warn you that your account will be completely removed in %period unless you log into the site before that time.

  Please visit us at %siteurl.

Sincerely,
-Randy Fay
Warmshowers.org webmaster',
  'inactive_user_notify_text' => 'Hej %username,

Du har inte logga in på Warmshowers.org sedan %lastaccess, och vi saknar dig! Vi ber alla att logga in åtminstone en gång varje år för att säkerställa att informationen på hemsidan stämmer. Därför skulle vi nu vilja att du loggar in för att uppdatera ditt konto, eller åtminstone logga in för att visa att du fortfarande tar emot mail på denna adress.

Se till att snart uppdatera dina uppgifter på %siteurl. Om du inte loggar in på ett helt år kommer ditt konto så småningom att raderas (du kommer att få mer än denna förvarning innan det händer).

Om du har glömt ditt lösenord kan du begära en engångsinloggning på http://www.warmshowers.org/user/password. Skriv in din mailadress så skickar vi ett mail med inloggningslänken.

Med vänliga hälsningar,

-Randy Fay

Warmshowers.org
------
Admin Information
Search: http://www.warmshowers.org/search/wsuser/%useremail
Username: %username
Email: %useremail',
  'node_notify_default_mailtext' => 'Hej [comment:node:author:fullname],

Du har fått en kommentar på: "[comment:node:title]":

----
[comment:title]
[comment:body]
----

Du kan visa kommentaren genom att <a href=”[comment:url]">klicka här</a>. (Svara inte på detta mail då det endast når ansvariga för hemsidan.)

Du kommer att få mail som detta varje gång någon svarar på dina trådar.

- Warmshowers-gruppen',
  'user_mail_password_reset_body' => 'Hej [user:name],

Du (eller någon annan) har begärt en engångsinloggning for ditt konto på
Warmshowers.org. Om det inte var du som gjorde det, kan du ignorera detta mail.

<a href=”[user:one-time-login-url]">Klicka här för att logga in och återställa ditt lösenord</a>. Om du inte
kan klicka på länken, kan du istället kopiera och klistra in denna URL i din webbläsare:

[user:one-time-login-url]

Det här är en engångsinloggning, som bara kan utnyttjas vid ett tillfälle. Länken går ut
efter en dag, och skulle den inte användas händer ingenting.

Efter inloggningen kommer du att bli vidarebefordrad till <a href=”[user:edit-url]”>dina kontoinställningar</a> så att du kan ändra ditt lösenord.

Tack,

-Randy Fay och registreringsgruppen

WarmShowers.org

Felsökning:

Om du kommer till "Ledsen - du har inte tillgång till den här sidan (error 403)", betyder det troligtvis att du har cookies avstängda i din webbläsare. För att göra rätt cookies-inställningar kan du läsa om detta bland våra<a href=”http://www.warmshowers.org/content/i-try-log-it-still-says-im-not-logged”>Vanliga frågor</a>.

Om ditt problem kvarstår, kan du svara på detta mail så ska vi försöka hjälpa dig.',
  'user_mail_password_reset_subject' => '[Warmshowers.org] Länk till engångsinloggning för [user:name] ',
  'user_mail_register_admin_created_body' => '[user:name],

A site administrator at !site has created an account for you. You may now log in to !login_uri using the following username and password:

username: [user:name]

You may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you will be redirected to [user:edit-url] so you can change your password.


--  !site team',
  'user_mail_register_admin_created_subject' => 'An administrator created an account for you at  !site',
  'user_mail_register_no_approval_required_body' => '[user:name],

Tack för din registrering på Warmshowers.org.

<a href=”[user:one-time-login-url]">Klicka här</a> för att bekräfta din mailadress och aktivera ditt konto.
(Om du inte kan klicka på länken kan du istället kopiera och klistra in denna länk i din
webbläsare:

[user:one-time-login-url]

Efter att du bekräftat ditt konto genom länken ovan kommer du att kunna logga in på ditt
konto med följande användarnamn och lösenord:

användarnamn: [user:name]
lösenord: Lösenordet du valde vid registreringen

Om du har några frågor kan du svara på detta mail så ska hjälper vi dig på bästa sätt.

Tack,


-- !site team

--Admin use only
Admin (edit): <a href="[user:edit-url]">[user:edit-url]</a> ',
  'user_mail_register_no_approval_required_subject' => '[Warmshowers.org] Bekräfta ditt konto på Warmshowers.org',
  'user_mail_register_pending_approval_body' => 'Welcome [user:name],

*** You must reply to this email to get access to the site! ***
*** Please reply saying something about yourself! ***
*** You must leave the message\'s subject and the rest of this message as they are. ***

( And when you reply - we need you to say "hi". You\'re not replying to a machine but to Randy and the rest of the team! )

*** You won\'t be able to log in until we enable your account
*** which is usually at least several hours after you send your reply.

Thank you for your registration at www.warmshowers.org.

Thank you for applying for an account, and for your hospitality to touring cyclists.

You need to acknowledge that you\'ve received this message by replying to it and saying something about yourself so we can correspond with you and try to make sure everybody on the list is a bona fide member.

I will enable your account after I receive your response.

(Minors - people not of legal age - are not allowed to use the site. So if you are under age, please do not continue.)

I\'ll be happy to approve your account. Your username and password (active after we correspond) are below.

Randy Fay
Warmshowers.org webmaster

AFTER your account has been authorized (after I receive your response and
activate your account), your username and password details will be:

username: [user:name]


AFTER your account is approved, you may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you can always change your password and other account details at  [user:edit-url]

Also: After you log in, please go to "my account" and the "map my location" tab and make sure your location shows correctly on the map. There are instructions on that page that will help you set it.

Thanks for signing up!

-Randy Fay and the registration team
Warmshowers.org

--
Administrator use only:
[user:edit-url]',
  'user_mail_register_pending_approval_subject' => 'Account info for  [user:name] at WarmShowers.org-You must reply and say something about yourself',
  'user_mail_status_activated_body' => 'Välkommen [user:name],

Ditt konto på www.warmshowers.org har blivit aktiverat.

Du kan logga in med ditt befintliga användarnamn och lösenord, eller genom att <a href=”[user:one-time-login-url]">klicka här</a>.

Detta är en engångsinloggning, och kan bara användas vid ett tillfälle.

Efter inloggningen kommer du att vidarebefordras till <a href=”[user:edit-url]">dina kontoinställningar</a> där du har möjlighet att ändra ditt lösenord.

Några frågor? Svara på detta meddelande så hjälper vi dig.

Tack,

-Randy Fay

Warmshowers.org

--

Admin use only:

!uri',
  'user_mail_status_activated_subject' => '[Warmshowers.org] Ditt konto har godkänts',
  'user_mail_status_blocked_body' => 'Hello [user:name],

Your account on www.warmshowers.org has been blocked.

If you have any questions, please reply to this email.

Thanks,
Randy Fay
Warmshowers.org webmaster

Admin use:
[user:edit-url]',
  'user_mail_status_blocked_subject' => 'Warmshowers.org: [user:name] account blocked',
  'user_mail_status_deleted_body' => 'Hej [user:name],

Ditt konto på www.warmshowers.org har blivit raderat, troligtvis på din egen begäran eller som till följd av att kontot verkar vara ogiltigt eller skapat som spam.

Om du har några frågor, svar på detta meddelande så hjälper vi dig.

Tack,

Randy Fay

Warmshowers.org',
  'user_mail_status_deleted_subject' => '[Warmshowers.org] [user:name] - konto raderat',
  'user_picture_guidelines' => 'Vi vill gärna ha en bild på <i>dig</i>, kanske när du cyklar eller är du bor. Det här är så klart helt frivilligt men kan vara roligt. Slumpmässiga bilder visas även på förstasidan. <i>Om du laddar upp en ny bild och den inte visas direkt, testa att uppdatera sidan så ska den dyka upp. (Håll ner skift-tangenten och klicka på R på tangentbordet.)</i>',
  'user_registration_help' => '<h3>Välkommen! Var god läs igenom detta innan du fortsätter.</h3> Tack för att du vill vara en del av Warm Showers! Alldeles strax kommer du att få ange dina uppgifter, därefter kommer vi att skicka ett bekräftelsemail med en länk som du behöver klicka på. För att undvika att vårt mail hamnar i ditt spam-filter är det säkrast att lägga till vår mailadress i din adressbok. Om inte, kanske du inte får mailet vilket betyder att ditt konto inte kan aktiveras. Därför…  <ol><li> Innan du fyller i formuläret, var god lägg till wsl@warmshowers.org i din adressbok. Instruktioner för hur man vanligtvis gör det finns <a href=”/spam_filters" target="_blank">här</a>. Om du inte gör detta, kanske du inte får bekräftelsemailet med ditt lösenord som behövs för att bli registrerad.</li> <li>Läs igenom <a href="/the_rules" target=”_blank">våra regler</a> och kom ihåg att gå tillbaka till dem om du känner dig osäker på något.</li> <li>När du registrerar dig, kom ihåg att det betyder att du planerar att erbjuda ditt värdskap, åtminstone någon gång i framtiden. Glöm inte att det här är en hemsida för ömsesidig gästvänlighet. (Om du är på resande fot eller av annan anledning inte kan vara värd för tillfället kryssar du i ”för tillfället ej tillgänglig”-rutan för att senare kryssa ur rutan vid tillfälle.) </li> <li>Minderåriga är inte tillåtna att använda denna sida, så fortsätt inte om inte har åldern inne.</li> <li>Kom ihåg att vi har en <a href="/faq" target=”_blank">Vanliga frågor</a> sida, som kanske kan svara på eventuella frågor du har. Kom ihåg att leta där först om du någon gång får problem med att logga in på sidan.</li> <li>Är du oroad över vem som kan se informationen du lägger upp? Hemsidan är konstruerad på så vis att din profil enbart kan visas av inloggade medlemmar. Din mailadress är alltid dold, även för de andra medlemmarna.</li> <li>Din personliga säkerhet är väldigt viktig för oss. Läs igenom sidan <a href=”/personal-security">Personlig säkerhet</a> för mer information om ämnet</li> <li>Vår <a target="_blank" href="/privacy">Sekretesspolicy</a> påminner dig om att din profil endast är synlig för andra medlemmar och att bara medlemmar kan kontakta dig via hemsidan.</li> <li>Genom att registrera dig accepterar du våra<a target="_blank" href="/terms">Användarvillkor</a> vilka betonar att <strong>du ensam ansvarar för dina interaktioner med andra Warmshowers-medlemmar.</strong></li> </ol> <br/><br/> Det var allt! Fortsätt nedan för att fullfölja din registrering. <hr/>',
);
  $realm_variables['language']['tr'] = array(
  'contact_form_information' => '<p>You are welcome to contact us here via email.</p>

<p>If you need help using the site, please <a href="/faq">read the FAQ first</a>, and then please consider reading the <a href="http://www.warmshowers.org/forums/website-help-and-support">Help Forum</a> and posting there where others can help you, and the answer will help others as well.</p>

<p>If you speak English it will help the admins a *lot* if you can ask your question in English. Thanks! </p>',
  'wsuser_become_available_notification_body' => 'Hi [user:fullname] - <br/><br/>Your Warmshowers.org account, [user], has been automatically updated to show you as "available to host" because the date that you specified you would become available after has passed. If you are still unavailable to host please log into Warmshowers.org and update your account. You can log in by visiting the site or just <a href=\'[user:login_url]\'>clicking this link</a>. Thanks!<br/><br/>-Randy Fay<br/>Warmshowers.org Webmaster<br/>---<br/><a href=\'[user:url]\'>Profile link</a><br/><a href=\'[account-edit-url]\'>Edit profile link</a><br/><a href=\'[user:login_url]\'>Direct login link</a><br/>',
  'wsuser_become_available_notification_subject' => 'You are now listed as available to host on Warmshowers.org ([user:fullname])',
  'wsuser_notavailable_reminder_body' => 'Hi [user:fullname], <br/><br/>Your warmshowers.org account, [user], is marked as unavailable to host. Could you please log in and set the date that you expect to return? If you don\'t know when or if you\'ll be able to host, just put in a date like 2025-01-01. You can change it at any time. You can log in with your username and password at <a href=\'https://tr.warmshowers.org\'>tr.warmshowers.org</a> or <a href=\'[user:login_url]\'>click this link to log in automatically</a><br/><br/>-Randy Fay<br/>Warmshowers.org Webmaster<br/>---<br/><a href=\'[user:url]\'>Profile link</a><br/><a href=\'[account-edit-url]\'>Edit profile link</a><br/><a href=\'[user:login_url]\'>Direct login link</a><br/>',
  'wsuser_notavailable_reminder_subject' => 'Your Warmshowers account is marked unavailable. Please update your return date. ([user:fullname])',
);
  $realm_variables['language']['zh-hans'] = array(
  'comment_notify_default_mailtext' => '[comment:node:author:fullname]，您好！

[comment:author:fullname] 回复了主题： "[comment:node:title]":

----
[comment:title]
[comment:body]
----

如果您需要查看和回复主题，请<a href="[comment:url]">点击这里</a>（请不要回复这封邮件，会发送给网站管理员滴）。

如果您不想再收到关于此主题回复的邮件提醒，请<a href="[comment-subscribed:unsubscribe-url]">点击这里</a>。

- Warmshowers.org团队敬上
',
  'contact_form_information' => '<p>欢迎通过电子邮件联系我们。</p>

<p>如果您对网站使用不太熟悉，请先阅读 <a href="/faq">常见问题和答案(FAQ) </a>，如果问题还未解决，请在<a href="http://www.warmshowers.org/forums/website-help-and-support">帮助讨论</a>中提问，热心人士会及时回答您的问题，同时答案也能帮助到其他人。</p>

<p>If you speak English it will help the admins a *lot* if you can ask your question in English. Thanks! </p>',
  'inactive_user_block_notify_text' => 'Hello %username,

  This automatic message is to notify you that your account on %sitename has been automatically disabled due to no activity for more than %period.

  Please visit us at %siteurl to have your account re-enabled.

Sincerely,
  %sitename team',
  'inactive_user_block_warn_text' => 'Hello %username,

  We haven\'t seen you at %sitename since %lastaccess, and we miss you!  This automatic message is to warn you that your account will be disabled in %period unless you come back and visit us before that time.

  Please visit us at %siteurl.

Sincerely,
  %sitename team',
  'inactive_user_delete_notify_text' => 'Hello %username,

  This automatic message is to notify you that your Warmshowers.org account has been automatically removed because you didn\'t log in for more than %period.

  Please visit us at %siteurl if you would like to create a new account.

Sincerely,
-Randy Fay
Warmshowers.org Webmaster',
  'inactive_user_delete_warn_text' => 'Hello %username,

Your account at Warmshowers.org will soon be removed (in %period) unless you log in to let us know you still want it. Please log into the site at http://www.warmshowers.org and update your account.  If you don\'t know your username or password that\'s OK - you can log in with your email address. If you don\'t know your password, please visit http://www.warmshowers.org/user/password to get a one-time login link emailed to you.

You haven\'t logged in %lastaccess.   This automatic message is to warn you that your account will be completely removed in %period unless you log into the site before that time.

  Please visit us at %siteurl.

Sincerely,
-Randy Fay
Warmshowers.org webmaster',
  'inactive_user_notify_text' => '亲爱的%username：

您已经很长时间没有登录Warmshowers.org啦！您上次登录的时间是%lastaccess，
我们很想您。我们希望网站用户至少能每年登录网站一次，以保证自己的联系信息是准确的。如果您确认自己的联系信息一直没变，请至少登录一次，让系统知道您还能收到我们的邮件。

请立即访问%siteurl更新您的信息，或者登录一次。如果您在一年内（365天）未登录，您的帐号则会被删除（在此我们删除您的帐号之前，我们会通过不少于一封邮件提醒您）。

如果您忘记了密码，请访问网页http://www.warmshowers.org/user/password，在页面中输入您的邮箱地址(%useremail)，您可以重置网站的登录密码。

此致 敬礼！
Randy Fay
Warmshowers.org

管理者信息
搜索: http://www.warmshowers.org/search/wsuser/%useremail
用户名: %username
E-mail: %useremail
',
  'node_notify_default_mailtext' => '[comment:node:author:fullname]，您好！

[comment:author:fullname] 回复了您发表的主题 "[comment:node:title]"：

----
[comment:title]
[comment:body]
----


如果您需要查看回复，请<a href="[comment:url]">点击这里</a>（请不要回复这封邮件，会发送给网站管理员滴）。

-Warmshowers.org团队敬上
',
  'user_mail_password_reset_body' => '[user:name]，您好

您（也有可能是其他人）刚刚申请了重置您在WarmShowers网站的登录密码。如果这不是您进行的申请，请忽略这封邮件。

请点击 <a href="[user:one-time-login-url]">这里 登录并修改您的密码</a>，如果此链接不能点击，请复制下面的链接到您浏览器的地址栏，并按下回车键。

密码重置的链接是一次性的，24小时后自动过期，如果不点击，密码则不会被修改。

点击链接后，您将会被重新引导至<a href="[user:edit-url]">编辑您的档案</a> 页面，这时您就能重置您的密码了。

此致 敬礼！
-Randy Fay 以及注册团队
WarmShowers.org


提示：
如果您得到“非常抱歉，您没有权限访问此页（403错误）”，这说明您已经关闭了浏览器的Cookies。如何打开浏览器的Cookies，请参阅FAQ页面的<a href="http://www.warmshowers.org/content/i-try-log-it-still-says-im-not-logged">这篇文章</a>。

如果您打开了浏览器的Cookies仍不能解决问题，请直接回复这封邮件寻求我们的帮助。',
  'user_mail_password_reset_subject' => '[Warmshowers.org] 重置 [user:name] 的密码',
  'user_mail_register_admin_created_body' => '[user:name],

A site administrator at !site has created an account for you. You may now log in to !login_uri using the following username and password:

username: [user:name]


You may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you will be redirected to [user:edit-url] so you can change your password.


--  !site team',
  'user_mail_register_admin_created_subject' => 'An administrator created an account for you at  !site',
  'user_mail_register_no_approval_required_body' => '[user:name],


非常感谢您注册 Warmshowers.org

请点击 <a href="[user:one-time-login-url]">这里</a>验证您的邮件地址，激活您的帐号（如果此链接不能点击，请复制下面的链接到您浏览器的地址栏，并按下回车键）。

[user:one-time-login-url]

在完成验证后，您可以使用下面的用户名登录并且使用WarmShowers了：

用户名: [user:name]
密码: 您在注册时输入的密码

如果您有任何疑问，请直接回复这封邮件，我们将尽最大的努力帮助您！

此致 敬礼！

--仅供管理员使用
管理员 (编辑): <a href="[user:edit-url]">[user:edit-url]</a>',
  'user_mail_register_no_approval_required_subject' => '[Warmshowers.org]请验证您在Warmshowers.org的帐号',
  'user_mail_register_pending_approval_body' => 'Welcome [user:name],

*** You must reply to this email to get access to the site! ***
*** Please reply saying something about yourself! ***
*** You must leave the message\'s subject and the rest of this message as they are. ***

( And when you reply - we need you to say "hi". You\'re not replying to a machine but to Randy and the rest of the team! )

*** You won\'t be able to log in until we enable your account
*** which is usually at least several hours after you send your reply.

Thank you for your registration at www.warmshowers.org.

Thank you for applying for an account, and for your hospitality to touring cyclists.

You need to acknowledge that you\'ve received this message by replying to it and saying something about yourself so we can correspond with you and try to make sure everybody on the list is a bona fide member.

I will enable your account after I receive your response.

(Minors - people not of legal age - are not allowed to use the site. So if you are under age, please do not continue.)

I\'ll be happy to approve your account. Your username and password (active after we correspond) are below.

Randy Fay
Warmshowers.org webmaster

AFTER your account has been authorized (after I receive your response and
activate your account), your username and password details will be:

username: [user:name]


AFTER your account is approved, you may also log in by clicking on this link or copying and pasting it in your browser:

[user:one-time-login-url]

This is a one-time login, so it can be used only once.

After logging in, you can always change your password and other account details at  [user:edit-url]

Also: After you log in, please go to "my account" and the "map my location" tab and make sure your location shows correctly on the map. There are instructions on that page that will help you set it.

Thanks for signing up!

-Randy Fay and the registration team
Warmshowers.org

--
Administrator use only:
[user:edit-url]',
  'user_mail_register_pending_approval_subject' => 'Account info for  [user:name] at WarmShowers.org-You must reply and say something about yourself',
  'user_mail_status_activated_body' => '[Warmshowers.org] 您的帐号已被激活

亲爱的 [user:name]：

我们很高兴地通知您，您在 www.warmshowers.org的帐号已经被激活。
您现在可以使用您的用户名和密码登录了，或者您也可以点击<a href="[user:one-time-login-url]">这里</a>登录。
这个登录链接只能点击一次，点击一次后失效。
登录后，您将被引导至 <a href="[user:edit-url]">档案编辑页面</a>，您可以在这里修改密码。
还有其他问题吗？可以直接回复邮件提问。
此致 敬礼！
-Randy Fay
Warmshowers.org 站长

--
Admin use only:
!uri
',
  'user_mail_status_activated_subject' => '[Warmshowers.org] 您的帐号已被激活',
  'user_mail_status_blocked_body' => 'Hello [user:name],

Your account on www.warmshowers.org has been blocked.

If you have any questions, please reply to this email.

Thanks,
Randy Fay
Warmshowers.org webmaster

Admin use:
[user:edit-url]',
  'user_mail_status_blocked_subject' => 'Warmshowers.org: [user:name] account blocked',
  'user_mail_status_deleted_body' => '亲爱的 [user:name]：

您在www.warmshowers.org的帐号已经被删除，可能是以下的原因导致：您自己申请删除、无效账户、账户被盗或者其他原因。
如果您有疑问，请回复此邮件。

此致 敬礼！
Randy Fay
Warmshowers.org
',
  'user_mail_status_deleted_subject' => '[Warmshowers.org]您的账户 [user:name]已经被删除',
  'user_picture_guidelines' => '您可以上传<i>您自己</i>的一张照片，如果是您在当地骑车的有趣照片就最好啦！照片自愿上传，上传的照片也会随机显示在首页上。<i>如果您上传一张新照片后不能马上显示，请Shift+F5强制刷新页面。</i>',
  'user_registration_help' => '<h3>欢迎您的加入！在进行下一步操作之前，请仔细阅读下面的内容</h3>
我们非常高兴您能加入WarmShowers社区。在您填写完所有内容并且提交后，我们会发送给您一封验证E-mail，此邮件中包含一个验证链接，请点击此链接完成验证。如果不完成验证，您将无法登录使用WarmShowers网站。根据当时的服务器负载和网络情况，邮件一般会在几十秒至十几分钟左右发送给您，如果您没有收到新邮件，请检查您的垃圾邮件箱，如果还是没有，那就是被邮件服务器当作垃圾邮件过滤掉啦……

<ol><li>
在提交注册请求之前，请将wsl@warmshowers.org 添加到您的邮件白名单，点击 <a href="/spam_filters" target="_blank">这里</a>查看不同邮件服务提供商添加白名单的方法。如果您不这么做，您很有可能收不到邮件，那就意味着不能完成验证，不能使用WarmShowers啦~</li>
<li>请仔细阅读WarmShowers的<a href="/the_rules" target="_blank">使用原则（非常重要）</a> ，当您有疑问时，请随时查阅。</li>
<ol><li>
<li>一旦您注册之后，请记得这意味着您已经做好了接待骑行者的准备。网站能一直发挥作用的原因是：这是一个互相帮助的网站（如果您当前正在骑车旅行，或者因为自身原因不能接待骑行者，请记得在您的档案页面的“当前无法接待”复选框前勾上勾，当您可以接待骑行者时请一定记得将复选框前的勾去掉）。</li>
<li>未成年人（中国法定为18周岁以下）不得使用该网站。</li>
<li>当您在使用这个网站上有功能上的疑问时，请记得随时查阅<a href="/faq" target="_blank">常见问题及答案（FAQ）</a>页面。</li>
<li>请不要担心您的隐私问题。只有登录网站的会员才能看到您的信息，您的邮箱地址除了自己之外，任何人不可见。您的任何私人信息都不会被搜索引擎检索到。</li>
<li>即使这是一个信任度极高的社区，您的人身安全也是非常重要的，请仔细阅读<a href="/personal-security">人身安全</a>页面。</li>
<li>如果您有空，请阅读我们的<a target="_blank" href="/privacy">隐私协议</a>，您的私人信息只有WarmShowers的其他会员可见，也只有WarmShowers的会员才能通过您的私人信息联系您。</li>
<li>注册的同时，表示您已经同意我们<a target="_blank" href="/terms">服务条款</a> ，这意味着 <strong>您将独自承担与其他WarmShowers会员接触时所产生的一切后果</strong>。</li>
</ol>
<br/><br/>
以上是注意事项，接下来请继续完成您的注册。<hr/>',
  'wsuser_become_available_notification_body' => '[user:fullname]，您好！  <br/><br/>因为您之前的设置，您的Warmshowers帐户 [user] 已经被自动标记为您当前可以接待。如果您当前还处于不可接待状态，请登录Warmshowers修改您的设置。可以直接登录网站，或者<a href=\'[user:login_url]\'>点击这里</a>自动登录。非常感谢！<br/><br/>-Randy Fay<br/>Warmshowers管理员<br/>---<br/><a href=\'[user:url]\'>Profile link</a><br/><a href=\'[account-edit-url]\'>Edit profile link</a><br/><a href=\'[user:login_url]\'>直接登录网站</a><br/>',
  'wsuser_become_available_notification_subject' => ' [user:fullname]，您的Warmshowers帐户被标记为当前可以接待。',
  'wsuser_notavailable_reminder_body' => '您好！[user:fullname]。 <br/><br/>您的warmshowers.org帐户 [user] 已经被标记为当前不可接待。请登录网站设置您的可接待日期。如果您无法确定这个日期，请将其设置为类似于2025-01-01这样的日期。您可以随时修改。您可以点击以下链接使用Warmshowers的账号和密码登录网站<a href=\'https://cn.warmshowers.org\'>cn.warmshowers.org</a>，或者 <a href=\'[user:login_url]\'>点击这里自动登录</a><br/><br/>-Randy Fay<br/>Warmshowers.org管理员<br/>---<br/><a href=\'[user:url]\'>您的档案链接</a><br/><a href=\'[account-edit-url]\'>编辑档案链接</a><br/><a href=\'[user:login_url]\'>直接登录链接</a><br/>',
  'wsuser_notavailable_reminder_subject' => ' [user:fullname]，您的Warmshowers帐户被标记为当前不可接待，请更新您的可接待日期。 ',
);

return $realm_variables;
}
