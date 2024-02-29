let SessionLoad = 1
if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
inoremap <silent> <Plug>(neocomplete_auto_refresh) =neocomplete#mappings#refresh()
inoremap <expr> <BS> neocomplete#smart_close_popup()."\"
inoremap <silent> <Plug>(neocomplete_start_manual_complete) =neocomplete#mappings#manual_complete()=neocomplete#mappings#popup_post()
inoremap <silent> <Plug>(neocomplete_start_auto_complete) =neocomplete#mappings#auto_complete()=neocomplete#mappings#popup_post()
inoremap <silent> <Plug>(neocomplete_start_omni_complete) 
inoremap <silent> <expr> <Plug>(neocomplete_start_unite_quick_match) unite#sources#neocomplete#start_quick_match()
inoremap <silent> <expr> <Plug>(neocomplete_start_unite_complete) unite#sources#neocomplete#start_complete()
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
set ruler
set runtimepath=~/.vim,/var/lib/vim/addons,/usr/share/vim/vimfiles,/usr/share/vim/vim74,/usr/share/vim/vimfiles/after,/var/lib/vim/addons/after,~/.vim/after
set showtabline=2
set suffixes=.bak,~,.swp,.o,.info,.aux,.log,.dvi,.bbl,.blg,.brf,.cb,.ind,.idx,.ilg,.inx,.out,.toc
set tabline=%!airline#extensions#tabline#get()
set tabstop=4
set tags=tags;
set termencoding=utf-8
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
cd /work/projects/kdm2412_sdm660_181226/hardware
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
badd +3224 qcom/camera/QCamera2/HAL/QCamera2HWI.cpp
badd +1332 qcom/camera/QCamera2/stack/mm-camera-interface/src/mm_camera_channel.c
badd +163 qcom/camera/QCamera2/stack/mm-camera-interface/src/mm_camera_stream.c
badd +919 qcom/camera/QCamera2/stack/mm-camera-interface/src/mm_camera.c
badd +696 qcom/camera/QCamera2/stack/mm-camera-interface/src/mm_camera_interface.c
badd +393 qcom/camera/QCamera2/HAL/QCameraStream.cpp
badd +2112 qcom/camera/QCamera2/HAL/QCameraMem.cpp
badd +139 qcom/camera/QCamera2/HAL/QCameraMem.h
badd +2116 qcom/media/mm-core/inc/OMX_QCOMExtns.h
badd +173 qcom/camera/QCamera2/HAL/QCameraStream.h
badd +524 qcom/camera/QCamera2/stack/mm-camera-interface/inc/mm_camera.h
badd +305 qcom/camera/QCamera2/stack/mm-camera-interface/src/mm_camera_thread.c
badd +182 qcom/camera/QCamera2/HAL/QCameraChannel.cpp
badd +3741 qcom/camera/QCamera2/HAL/QCamera2HWICallbacks.cpp
badd +24 ~/OnTheWay/Android/camera/qcom_hal
badd +205 qcom/camera/QCamera2/HAL/QCamera2HWI.h
badd +940 qcom/camera/QCamera2/stack/common/mm_camera_interface.h
badd +105 libhardware/include/hardware/camera.h
badd +47 qcom/camera/QCamera2/HAL/QCameraAllocator.h
badd +1337 qcom/camera/QCamera2/HAL/QCameraParameters.h
badd +15588 qcom/camera/QCamera2/HAL/QCameraParameters.cpp
args \[BufExplorer]
edit qcom/camera/QCamera2/HAL/QCamera2HWI.cpp
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
if &filetype != 'cpp'
setlocal filetype=cpp
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
if &syntax != 'cpp'
setlocal syntax=cpp
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
let s:l = 3224 - ((15 * winheight(0) + 16) / 32)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
3224
normal! 0
lcd /work/projects/kdm2412_sdm660_181226/hardware
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
