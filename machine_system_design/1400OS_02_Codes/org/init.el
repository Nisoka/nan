;disable startup message
(setq inhibit-startup-message t)


;line number
(require 'linum)
(global-linum-mode t)

;cscope
(add-to-list 'load-path' "~/.emacs.d/xcscope")
(require 'xcscope)
(add-hook 'c-mode-common-hook '(lambda() (require 'xcscope)))
(setq cscope-do-not-update-database t)

(setq command-line-default-directory "C:/Users/people/QtProject/python")

;cedet
;(load-file "~/.emacs.d/install/cedet-1.1/common/cedet.el")
;(global-ede-mode 1)
;(semantic-load-enable-gaudy-code-helpers)
;(global-srecode-minor-mode 1)

;(setq max-lisp-eval-depth 3000)
;(setq max-specpdl-size 10000)

;ecb
;(add-to-list 'load-path "/home/nan/.emacs.d/install/ecb-2.40")
;(require 'ecb)
;(require 'ecb-autoloads)
;(setq stack-trace-on-error nil)
;(setq kill-ring-max 1024)


;(global-set-key (kbd "<f7>") 'ecb-minor-mode)  ;open ecb
 
 ;;keybind
(global-set-key [M-left] 'windmove-left) 
(global-set-key [M-right] 'windmove-right) 
(global-set-key [M-up] 'windmove-up) 
(global-set-key [M-down] 'windmove-down) 

;auto-comlate
(add-to-list 'load-path "~/.emacs.d/install/auto-complete")
(require 'auto-complete-config)
(add-to-list 'ac-dictionary-directories "~/.emacs.d/install/auto-complete/ac-dict")
(ac-config-default)



(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(column-number-mode t)
 '(tool-bar-mode nil))
(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(default ((t (:family "文泉驿等宽微米黑" :foundry "outline" :slant normal :weight normal :height 102 :width normal)))))


;;设置窗口位置为屏库左上角(0,0)
(set-frame-position (selected-frame) 50 0)

;org-mode 换行
(add-hook 'org-mode-hook (lambda () (setq truncate-lines nil)))


