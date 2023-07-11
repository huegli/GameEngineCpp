function! myspacevim#before() abort
    " Stuff goes here
endfunction

function! myspacevim#after() abort
    " Stuff goes here
    call SpaceVim#plugins#runner#reg_runner('cpp', {
        \ 'exe'      : 'make',
        \ 'opt'      : ['build'],
        \ 'usestdin' : 0,
        \ })
endfunction

