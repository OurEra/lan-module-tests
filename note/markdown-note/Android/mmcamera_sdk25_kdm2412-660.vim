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
nnoremap  :bn
nnoremap  :bp
nmap f :cs find f =expand("<cfile>")
nmap e :cs find e =expand("<cword>")
nmap t :cs find t =expand("<cword>")
nmap c :cs find c =expand("<cword>")
nmap g :cs find g =expand("<cword>")
nmap s :cs find s =expand("<cword>")
nmap <silent> # <Plug>MarkSearchPrev
nmap <silent> * <Plug>MarkSearchNext
noremap <silent> .C :call UnComment()
noremap <silent> .c :call Comment()
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
nnoremap <silent> tr :NERDTreeToggle
nnoremap <silent> tt :Tagbar
nnoremap <silent> w, :vertical resize +3
nnoremap <silent> w. :vertical resize -3
noremap <silent> <Plug>AirlineSelectNextTab :exe repeat(':tabn|', v:count1)
noremap <silent> <Plug>AirlineSelectPrevTab gT
noremap <silent> <Plug>AirlineSelectTab9 :9tabn
noremap <silent> <Plug>AirlineSelectTab8 :8tabn
noremap <silent> <Plug>AirlineSelectTab7 :7tabn
noremap <silent> <Plug>AirlineSelectTab6 :6tabn
noremap <silent> <Plug>AirlineSelectTab5 :5tabn
noremap <silent> <Plug>AirlineSelectTab4 :4tabn
noremap <silent> <Plug>AirlineSelectTab3 :3tabn
noremap <silent> <Plug>AirlineSelectTab2 :2tabn
noremap <silent> <Plug>AirlineSelectTab1 :1tabn
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
set runtimepath=~/.vim,/var/lib/vim/addons,/usr/share/vim/vimfiles,/usr/share/vim/vim74,/usr/share/vim/vimfiles/after,/var/lib/vim/addons/after,~/.vim/after
set showtabline=2
set suffixes=.bak,~,.swp,.o,.info,.aux,.log,.dvi,.bbl,.blg,.brf,.cb,.ind,.idx,.ilg,.inx,.out,.toc
set tabline=%!airline#extensions#tabline#get()
set tabstop=4
set tags=tags;
set termencoding=utf-8
set window=33
set nowrapscan
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
cd /work/projects/kdm2412_sdm660_181226/vendor/qcom/proprietary
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
badd +4134 mm-camera/mm-camera2/media-controller/mct/stream/mct_stream.c
badd +4188 mm-camera/mm-camera2/media-controller/mct/pipeline/mct_pipeline.c
badd +432 mm-camera/mm-camera2/media-controller/modules/pproc-new/cpp/cpp_hardware.c
badd +734 mm-camera/mm-camera2/media-controller/modules/pproc-new/cpp/cpp_thread.c
badd +864 mm-camera/mm-camera2/media-controller/modules/pproc-new/cpp/cpp_module_events.c
badd +160 mm-camera/mm-camera2/media-controller/modules/pproc-new/cpp/cpp_module.c
badd +239 mm-camera/mm-camera2/media-controller/modules/pproc-new/cpp/cpp_port.c
badd +1190 ~/OnTheWay/Android/camera/qcom_mmcamera
badd +3151 mm-camera/mm-camera2/media-controller/modules/pproc-new/pproc_module.c
badd +1 mm-camera/mm-camera2/media-controller/modules/pproc-new/pproc_port.c
badd +853 mm-camera/mm-camera2/media-controller/mct/module/mct_module.c
badd +92 mm-camera/mm-camera2/media-controller/mct/module/mct_module.h
badd +17 mm-camera/mm-camera2/media-controller/includes/media_controller.h
badd +576 mm-camera/mm-camera2/media-controller/mct/controller/mct_controller.c
badd +131 mm-camera/mm-camera2/media-controller/mct/controller/mct_controller.h
badd +24 mm-camera/mm-camera2/media-controller/mct/tools/mct_queue.h
badd +309 mm-camera/mm-camera2/media-controller/mct_shim_layer/mct_shim_layer.c
badd +90 mm-camera/mm-camera2/media-controller/mct/bus/mct_bus.c
badd +3594 mm-camera/mm-camera2/media-controller/modules/sensors/module/module_sensor.c
badd +352 mm-camera/mm-camera2/media-controller/modules/isp2/module/isp_module.c
badd +593 mm-camera/mm-camera2/media-controller/modules/stats/stats_module.c
badd +1987 mm-camera/mm-camera2/media-controller/modules/imglib/modules/module_imglib.c
badd +148 mm-camera/mm-camera2/media-controller/mct/object/mct_object.c
badd +55 mm-camera/mm-camera2/media-controller/mct/stream/mct_stream.h
badd +700 mm-camera/mm-camera2/media-controller/mct/port/mct_port.c
badd +277 mm-camera/mm-camera2/media-controller/modules/isp2/hw/sce/sce40/module_sce40.c
badd +3067 mm-camera/mm-camera2/media-controller/modules/isp2/module/isp_resource.c
badd +110 mm-camera/mm-camera2/media-controller/modules/isp2/hw/sce/sce40/0310/sce40_0310.c
badd +1637 mm-camera/mm-camera2/media-controller/modules/isp2/hw/sce/sce40/sce40.c
badd +2 mm-camera/mm-camera2/media-controller/modules/isp2/hw/sub_module/isp_sub_module.c
badd +497 mm-camera/mm-camera2/media-controller/modules/isp2/module/isp_module.h
badd +872 mm-camera/mm-camera2/media-controller/modules/isp2/module/isp_port.c
badd +125 mm-camera/mm-camera2/media-controller/modules/isp2/module/isp_resource.h
badd +1 mm-camera/mm-camera2/media-controller/modules/isp2/module/isp_pipeline.h
badd +1 mm-camera/mm-camera2/media-controller/modules/isp2/module/isp48/isp_pipeline48.c
badd +71 mm-camera/mm-camera2/media-controller/modules/isp2/common/isp_log.h
badd +4 mm-camera/mm-camera2/media-controller/modules/isp2/module/Android.mk
badd +2418 mm-camera/mm-camera2/media-controller/modules/isp2/hw/sub_module/isp_sub_module_port.c
badd +24 mm-camera/mm-camera2/media-controller/mct/event/mct_event.h
badd +1331 mm-camera/mm-camera2/media-controller/modules/isp2/module/isp_trigger_thread.c
badd +7320 mm-camera/mm-camera2/media-controller/modules/isp2/module/isp_util.c
badd +214 mm-camera/mm-camera2/media-controller/modules/isp2/module/isp_hw_update_thread.c
badd +871 mm-camera/mm-camera2/media-controller/modules/isp2/module/isp_parser_thread.c
badd +178 mm-camera/mm-camera2/media-controller/modules/isp2/module/isp_hw_update_util.c
badd +570 mm-camera/mm-camera2/media-controller/modules/isp2/module/isp48/isp_pipeline48_parser.c
badd +30 mm-camera/mm-camera2/media-controller/modules/isp2/hw/sub_module/isp_sub_module_port.h
badd +1107 mm-camera/mm-camera2/media-controller/modules/isp2/common/isp_common.h
badd +31 mm-camera/mm-camera2/media-controller/modules/isp2/module/isp_port.h
badd +42 mm-camera/mm-camera2/media-controller/mct/object/mct_object.h
badd +463 mm-camera/mm-camera2/media-controller/modules/sensors/module/sensor_common.h
badd +39 mm-camera/mm-camera2/media-controller/modules/sensors/eebin/module/eebin_interface.c
badd +643 mm-camera/mm-camera2/media-controller/modules/sensors/eebin/module/eebin.c
badd +168 mm-camera/mm-camera2/media-controller/modules/sensors/module/sensor_init.c
badd +161 mm-camera/mm-camera2/media-controller/modules/sensors/configs/sdm660_kdm2412mx_camera.xml
badd +161 mm-camera/mm-camera2/media-controller/modules/iface2/module_iface.c
badd +35 mm-camera/mm-camera2/media-controller/mct/tools/mct_list.h
badd +201 mm-camera/mm-camera2/media-controller/modules/sensors/module/port_sensor.c
badd +5295 mm-camera/mm-camera2/media-controller/modules/sensors/sensor/module/sensor.c
badd +401 mm-camerasdk/sensor/includes/sensor_lib.h
badd +49 mm-camera/mm-camera2/media-controller/modules/sensors/module/module_sensor.h
badd +845 mm-camera/mm-camera2/media-controller/modules/sensors/module/sensor_util.c
badd +8 mm-camera/mm-camera2/media-controller/modules/sensors/sensor/libs/imx327/imx327_lib.c
badd +198 mm-camera/mm-camera2/media-controller/mct/port/mct_port.h
badd +10585 mm-camera/mm-camera2/media-controller/modules/iface2/iface_util.c
badd +1 mm-camera/mm-camera2/media-controller/modules/iface2/axi/iface_axi.c
badd +1727 mm-camera/mm-camera2/media-controller/modules/iface2/iface.c
badd +1 mm-camera/mm-camera2/media-controller/modules/iface2/axi/iface_axi_thread.c
badd +229 mm-camera/mm-camera2/media-controller/modules/iface2/port_iface.c
badd +201 mm-camera/mm-camera2/media-controller/modules/iface2/iface_session_thread.c
badd +433 mm-camera/mm-camera2/media-controller/modules/sensors/sensor/libs/imx327/imx327_lib.h
badd +60 mm-camera/mm-camera2/media-controller/modules/isp2/module/isp_util.h
badd +390 mm-camera/mm-camera2/media-controller/mct/tools/mct_list.c
badd +419 mm-camera/mm-camera2/media-controller/modules/iface2/iface.h
badd +20 mm-camera/mm-camera2/media-controller/modules/isp2/hw/ltm/ltm47/module_ltm47.c
badd +267 mm-camera/mm-camera2/media-controller/modules/isp2/hw/ltm/ltm47/ltm47.h
badd +3639 mm-camera/mm-camera2/media-controller/modules/stats/q3a/aec/aec_port.c
badd +18 mm-camera/mm-camera2/media-controller/modules/stats/q3a/aec/keda_aec_interface.c
args \[BufExplorer]
edit mm-camera/mm-camera2/media-controller/modules/stats/q3a/aec/aec_port.c
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
if &filetype != 'c'
setlocal filetype=c
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
setlocal shiftwidth=8
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
if &syntax != 'c'
setlocal syntax=c
endif
setlocal tabstop=4
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal noundofile
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 3639 - ((15 * winheight(0) + 16) / 32)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
3639
normal! 0
lcd /work/projects/kdm2412_sdm660_181226/vendor/qcom/proprietary
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
