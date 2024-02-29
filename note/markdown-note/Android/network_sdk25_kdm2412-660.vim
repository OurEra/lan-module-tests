let SessionLoad = 1
if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
inoremap <silent> <expr> <Plug>(neocomplete_start_unite_complete) unite#sources#neocomplete#start_complete()
inoremap <silent> <expr> <Plug>(neocomplete_start_unite_quick_match) unite#sources#neocomplete#start_quick_match()
inoremap <silent> <Plug>(neocomplete_start_omni_complete) 
inoremap <silent> <Plug>(neocomplete_start_auto_complete) =neocomplete#mappings#auto_complete()=neocomplete#mappings#popup_post()
inoremap <silent> <Plug>(neocomplete_start_manual_complete) =neocomplete#mappings#manual_complete()=neocomplete#mappings#popup_post()
inoremap <expr> <BS> neocomplete#smart_close_popup()."\"
inoremap <silent> <Plug>(neocomplete_auto_refresh) =neocomplete#mappings#refresh()
imap <S-Tab> 
inoremap <C-Tab> 	
inoremap <silent> <Plug>snipMateShow =snipMate#ShowAvailableSnips()
inoremap <silent> <Plug>snipMateBack =snipMate#BackwardsSnippet()
inoremap <silent> <Plug>snipMateTrigger =snipMate#TriggerSnippet(1)
inoremap <silent> <Plug>snipMateNextOrTrigger =snipMate#TriggerSnippet()
imap <silent> <expr> <F5> (pumvisible() ? "\<Plug>LookupFileCE" : "")."\\<Plug>LookupFile"
inoremap <Plug>LookupFileCE 
xmap 	 <Plug>snipMateVisual
smap 	 <Plug>snipMateNextOrTrigger
nmap  [m
nnoremap  :bn
nnoremap  :bp
nmap f :cs find f =expand("<cfile>")
nmap e :cs find e =expand("<cword>")
nmap t :cs find t =expand("<cword>")
nmap c :cs find c =expand("<cword>")
nmap g :cs find g =expand("<cword>")
nmap s :cs find s =expand("<cword>")
nmap  g
nmap <silent> # <Plug>MarkSearchPrev
nmap <silent> * <Plug>MarkSearchNext
noremap <silent> .C :call UnComment()
noremap <silent> .c :call Comment()
nmap <silent> W :WMToggle
nmap <silent> \ll :LUBufs
nmap <silent> \lw :LUWalk
nmap <silent> \? <Plug>MarkSearchAnyPrev
nmap <silent> \/ <Plug>MarkSearchAnyNext
nmap <silent> \# <Plug>MarkSearchCurrentPrev
nmap <silent> \* <Plug>MarkSearchCurrentNext
nmap <silent> \n <Plug>MarkClear
vmap <silent> \r <Plug>MarkRegex
nmap <silent> \r <Plug>MarkRegex
vmap <silent> \m <Plug>MarkSet
nmap <silent> \m <Plug>MarkSet
nmap \ihn :IHN
nmap \is :IHS:A
nmap \ih :IHS
map \c :echo g:colors_name
map \p :CP
nmap gx <Plug>NetrwBrowseX
omap tr :NERDTreeToggle
vmap tr :NERDTreeToggle
nnoremap <silent> tr :NERDTreeToggle
nnoremap <silent> tt :Tagbar
nnoremap <silent> w, :vertical resize +3
nnoremap <silent> w. :vertical resize -3
map <silent> <S-Left> : tabp
map <silent> <S-Right> : tabn
smap <S-Tab> <Plug>snipMateBack
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#NetrwBrowseX(expand("<cWORD>"),0)
snoremap <silent> <Plug>snipMateBack a=snipMate#BackwardsSnippet()
snoremap <silent> <Plug>snipMateNextOrTrigger a=snipMate#TriggerSnippet()
nmap <silent> <F5> <Plug>LookupFile
inoremap <expr>  neocomplete#smart_close_popup()."\"
imap 	 
imap 	 <Plug>snipMateShow
imap \ihn :IHN
imap \is :IHS:A
imap \ih :IHS
let &cpo=s:cpo_save
unlet s:cpo_save
set paste
set background=dark
set backspace=indent,eol,start
set cindent
set completeopt=preview,menuone
set cscopeprg=/usr/bin/cscope
set cscopetag
set cscopetagorder=1
set cscopeverbose
set expandtab
set fileencodings=utf-8,ucs-bom,gb18030,gbk,gb2312,cp936
set helplang=en
set history=50
set hlsearch
set ignorecase
set laststatus=2
set nomodeline
set printoptions=paper:a4
set report=10000
set runtimepath=~/.vim,/var/lib/vim/addons,/usr/share/vim/vimfiles,/usr/share/vim/vim74,/usr/share/vim/vimfiles/after,/var/lib/vim/addons/after,~/.vim/after
set shiftwidth=4
set showtabline=2
set suffixes=.bak,~,.swp,.o,.info,.aux,.log,.dvi,.bbl,.blg,.brf,.cb,.ind,.idx,.ilg,.inx,.out,.toc
set tabline=%!airline#extensions#tabline#get()
set tabstop=4
set tags=tags;
set termencoding=utf-8
set viminfo=!,'100,<50,s10,h
set nowrapscan
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
cd /work/projects/kdm2412_sdm660_181226/frameworks
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
badd +39 opt/net/ethernet/java/com/android/server/ethernet/EthernetService.java
badd +1 opt/net/ethernet/java/com/android/server/ethernet/EthernetServiceImpl.java
badd +426 opt/net/ethernet/java/com/android/server/ethernet/EthernetNetworkFactory.java
badd +1486 base/services/core/java/com/android/server/NetworkManagementService.java
badd +5195 base/services/core/java/com/android/server/ConnectivityService.java
badd +2443 opt/net/wifi/service/java/com/android/server/wifi/p2p/WifiP2pServiceImpl.java
badd +634 opt/net/wifi/service/java/com/android/server/wifi/WifiServiceImpl.java
badd +1801 base/services/core/java/com/android/server/connectivity/Tethering.java
badd +1 base/core/java/com/android/server/net/BaseNetworkObserver.java
badd +311 base/services/core/java/com/android/server/connectivity/tethering/TetherInterfaceStateMachine.java
badd +44 base/core/java/android/net/InterfaceConfiguration.java
badd +1566 base/wifi/java/android/net/wifi/WifiManager.java
badd +518 base/core/java/android/os/Handler.java
badd +72 base/core/java/android/os/Message.java
badd +136 base/core/java/android/os/Looper.java
badd +53 base/core/java/android/content/ServiceConnection.java
badd +5558 opt/net/wifi/service/java/com/android/server/wifi/WifiStateMachine.java
badd +1755 opt/net/wifi/service/java/com/android/server/wifi/WifiNative.java
badd +148 opt/net/wifi/service/java/com/android/server/wifi/scanner/HalWifiScannerImpl.java
badd +778 opt/net/wifi/service/java/com/android/server/wifi/WifiController.java
badd +173 opt/net/wifi/service/java/com/android/server/wifi/SoftApStateMachine.java
badd +90 opt/net/wifi/service/java/com/android/server/wifi/SoftApManager.java
badd +51 /work/projects/kdm2412_sdm660_181226/vendor/qcom/proprietary/qrdplus/Extension/apps/ExtSettings/src/com/qualcomm/qti/extsettings/WifiApSwitch.java
badd +325 base/core/java/android/os/MessageQueue.java
badd +974 opt/telephony/src/java/com/android/internal/telephony/dataconnection/DcTracker.java
badd +421 base/services/core/java/com/android/server/connectivity/NetworkAgentInfo.java
badd +272 ~/OnTheWay/Android/net/network
badd +434 opt/telephony/src/java/com/android/internal/telephony/dataconnection/DataConnection.java
badd +529 base/services/core/java/com/android/server/connectivity/NetworkMonitor.java
badd +183 base/core/java/android/net/NetworkCapabilities.java
badd +117 base/core/java/android/net/NetworkInfo.java
badd +1 ~/OnTheWay/Android/net/telephony
badd +240 ~/OnTheWay/Android/net/wifi
badd +1 opt/telephony/src/java/com/android/internal/telephony/GsmCdmaPhone.java
badd +132 opt/telephony/src/java/com/android/internal/telephony/PhoneFactory.java
badd +43 opt/telephony/src/java/com/android/internal/telephony/TelephonyComponentFactory.java
badd +190 base/telephony/java/android/telephony/TelephonyManager.java
badd +247 opt/telephony/src/java/com/android/internal/telephony/dataconnection/TelephonyNetworkFactory.java
badd +74 opt/telephony/src/java/com/android/internal/telephony/dataconnection/ApnContext.java
badd +44 opt/telephony/src/java/com/android/internal/telephony/dataconnection/DcAsyncChannel.java
badd +118 ~/OnTheWay/Android/net/ril+telephony/notes
badd +92 opt/telephony/src/java/com/android/internal/telephony/Phone.java
badd +415 opt/telephony/src/java/com/android/internal/telephony/dataconnection/DcController.java
badd +751 opt/telephony/src/java/com/android/internal/telephony/SubscriptionController.java
badd +274 opt/telephony/src/java/com/android/internal/telephony/ProxyController.java
badd +631 opt/telephony/src/java/com/android/internal/telephony/SubscriptionInfoUpdater.java
badd +1805 opt/telephony/src/java/com/android/internal/telephony/ServiceStateTracker.java
badd +1 opt/telephony/src/java/com/android/internal/telephony/uicc/UiccController.java
badd +513 opt/telephony/src/java/com/android/internal/telephony/uicc/IccCardProxy.java
badd +1 opt/telephony/src/java/com/android/internal/telephony/RIL.java
badd +1 opt/net/wifi/service/java/com/android/server/wifi/WifiQualifiedNetworkSelector.java
badd +1 opt/net/wifi/service/java/com/android/server/wifi/WifiConnectivityManager.java
badd +1086 opt/net/wifi/service/java/com/android/server/wifi/WifiMonitor.java
badd +796 ./opt/net/wifi/service/java/com/android/server/wifi/scanner/SupplicantWifiScannerImpl.java
badd +1304 ./opt/net/wifi/service/java/com/android/server/wifi/WifiConfigManager.java
badd +131 ./opt/net/wifi/service/java/com/android/server/wifi/SupplicantStateTracker.java
badd +371 ./opt/telephony/src/java/com/android/internal/telephony/PhoneSwitcher.java
badd +692 base/telephony/java/android/telephony/SubscriptionManager.java
badd +604 opt/telephony/src/java/com/android/internal/telephony/uicc/SIMRecords.java
badd +1 ./opt/telephony/src/java/com/android/internal/telephony/uicc/IccRecords.java
badd +109 ./opt/telephony/src/java/com/android/internal/telephony/uicc/UiccCardApplication.java
badd +412 ../vendor/qcom/proprietary/telephony-fwk/opt/telephony/src/java/com/qualcomm/qti/internal/telephony/QtiSubscriptionController.java
badd +363 ./base/telephony/java/com/android/internal/telephony/RILConstants.java
badd +328 ./opt/telephony/src/java/com/android/internal/telephony/PhoneInternalInterface.java
badd +374 ./opt/telephony/src/java/com/android/internal/telephony/BaseCommands.java
badd +1 opt/telephony/src/java/com/android/internal/telephony/uicc/IccCardStatus.java
badd +726 ./opt/telephony/src/java/com/android/internal/telephony/uicc/UiccCard.java
badd +142 ./base/telephony/java/android/telephony/ServiceState.java
badd +47 base/packages/SettingsLib/src/com/android/settingslib/wifi/WifiTracker.java
badd +1043 ../packages/apps/Settings/src/com/android/settings/wifi/WifiSettings.java
badd +1 ./opt/net/wifi/service/Android.mk
badd +8 ./base/services/Android.mk
badd +264 ./base/core/java/android/net/NetworkFactory.java
badd +1860 opt/net/wifi/service/java/com/android/server/wifi/scanner/WifiScanningServiceImpl.java
badd +1 ./base/wifi/java/android/net/wifi/WifiScanner.java
badd +65 ./opt/net/wifi/service/java/com/android/server/wifi/scanner/WifiScannerImpl.java
badd +1 ./opt/net/wifi/service/java/com/android/server/wifi/WifiConfigStore.java
badd +259 base/core/java/android/net/NetworkAgent.java
badd +305 ./base/core/java/android/net/NetworkRequest.java
args \[BufExplorer]
edit base/services/core/java/com/android/server/ConnectivityService.java
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winheight=1 winwidth=1
argglobal
inoremap <buffer> 	 =CodeComplete()=SwitchRegion()
setlocal keymap=
setlocal noarabic
setlocal noautoindent
setlocal balloonexpr=
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal cindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=s1:/*,mb:*,ex:*/,://,b:#,:%,:XCOMM,n:>,fb:-
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
setlocal nocursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'java'
setlocal filetype=java
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=tcq
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=2
setlocal imsearch=2
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal nolist
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal nomodeline
setlocal modifiable
setlocal nrformats=octal,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=4
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=0
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=%!airline#statusline(1)
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'java'
setlocal syntax=java
endif
setlocal tabstop=4
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal noundofile
setlocal nowinfixheight
setlocal nowinfixwidth
set nowrap
setlocal nowrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 5195 - ((15 * winheight(0) + 16) / 32)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
5195
normal! 0
tabnext 1
if exists('s:wipebuf')
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20 shortmess=filnxtToO
let s:sx = expand("<sfile>:p:r")."x.vim"
if file_readable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &so = s:so_save | let &siso = s:siso_save
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
