#include "mainwindow.h"
#include "ui_mainwindow.h"

#import <AppKit/AppKit.h>

#include <CoreGraphics/CGDataProvider.h>
#include <zzzrequest.h>

struct TouchBarButton {
    QString identifier;
    QString text;
    std::function<void(MainWindow*)> handler;
    QIcon icon;
};

struct MainWindowTouchBarHelper {
    static QMap<QString, TouchBarButton> touchBarButtons;
};


#define THEZZZ_TOUCH_BAR_VERB_BUTTON(verb) \
  { \
      QStringLiteral("com.vicr123.thezzz.verb." verb), \
      { \
        QStringLiteral("com.vicr123.thezzz.verb." verb), \
            QStringLiteral(verb).toUpper(),\
            [](MainWindow* mainWin) { \
                auto abstractRequest = mainWin->currentRequest(); \
                if (!abstractRequest) return; \
                auto request = abstractRequest.dynamicCast<ZzzRequest>(); \
                if (!request) return; \
                request->setVerb(QStringLiteral(verb).toUpper()); \
            } \
      } \
  }

QMap<QString, TouchBarButton> MainWindowTouchBarHelper::touchBarButtons = {
    {
        QStringLiteral("com.vicr123.thezzz.send-request"),
        {
            QStringLiteral("com.vicr123.thezzz.send-request"),
            MainWindow::tr("Send Request"),
            [](MainWindow* mainWin) {
                mainWin->executeCurrentRequest();
            },
            QIcon::fromTheme("mail-send")
        }
    },
    THEZZZ_TOUCH_BAR_VERB_BUTTON("get"),
    THEZZZ_TOUCH_BAR_VERB_BUTTON("post"),
    THEZZZ_TOUCH_BAR_VERB_BUTTON("put"),
    THEZZZ_TOUCH_BAR_VERB_BUTTON("patch"),
    THEZZZ_TOUCH_BAR_VERB_BUTTON("delete"),
    THEZZZ_TOUCH_BAR_VERB_BUTTON("options"),
    THEZZZ_TOUCH_BAR_VERB_BUTTON("head")
};

@interface MainWindowTouchBarProvider: NSResponder <NSTouchBarDelegate, NSApplicationDelegate, NSWindowDelegate>

@property (strong) NSObject *qtDelegate;
@property QObject *parentQObject;
@property MainWindow *mainWindow;

@property NSPopoverTouchBarItem* verbTouchBarItem;

@end

// Create identifiers for button items.
static NSTouchBarItemIdentifier verbIdentifier = @"com.vicr123.thezzz.verb";

@implementation MainWindowTouchBarProvider

- (id)init: (MainWindow*)mainWin  {
    if (self = [super init]) {
        self.parentQObject = new QObject();

        //Set main window UI to call touch bar handlers
        self.mainWindow = mainWin;
    }

    return self;
}

- (NSTouchBar *)makeTouchBar {
//    if (!self.touchBar) {
        // Create the touch bar with this instance as its delegate
        NSTouchBar *bar = [[NSTouchBar alloc] init];
        bar.delegate = self;

        bar.defaultItemIdentifiers = @[
          verbIdentifier,
          NSTouchBarItemIdentifierFlexibleSpace,
          @"com.vicr123.thezzz.send-request"
        ];
        bar.customizationRequiredItemIdentifiers = @[];
        //    bar.customizationAllowedItemIdentifiers = @[firstFrameIdentifier, lastFrameIdentifier, playIdentifier, inPointIdentifier, outpointIdentifier, renderIdentifier, timelineIdentifier, NSTouchBarItemIdentifierFlexibleSpace];
        [bar setCustomizationIdentifier:@"com.vicr123.thezzz.touchbar"];

//        self.touchBar = bar;
        return bar;
//    }
//    return self.touchBar;
}

- (NSPopoverTouchBarItem *)getVerbTouchBarItem {
    if (!self.verbTouchBarItem) {
        auto item =
            [[NSPopoverTouchBarItem alloc] initWithIdentifier:verbIdentifier];
        [item setCustomizationLabel:MainWindow::tr("Verb").toNSString()];
        [item setCollapsedRepresentationLabel:@"GET"];

        auto popoverTouchBar = [[NSTouchBar alloc] init];
        [popoverTouchBar setDefaultItemIdentifiers:@[
          @"com.vicr123.thezzz.verb.get",
          @"com.vicr123.thezzz.verb.post",
          @"com.vicr123.thezzz.verb.put",
          @"com.vicr123.thezzz.verb.patch",
          @"com.vicr123.thezzz.verb.delete",
          @"com.vicr123.thezzz.verb.options",
          @"com.vicr123.thezzz.verb.head",
        ]];
        [popoverTouchBar setDelegate:self];
        [item setPopoverTouchBar:popoverTouchBar];

        self.verbTouchBarItem = item;
    }

    return self.verbTouchBarItem;
}

- (NSTouchBarItem *)touchBar:(NSTouchBar *)touchBar makeItemForIdentifier:(NSTouchBarItemIdentifier)identifier {
    Q_UNUSED(touchBar);

    if ([identifier isEqualToString:verbIdentifier]) {
        return [self getVerbTouchBarItem];
    } else {
        auto buttonDetails = MainWindowTouchBarHelper::touchBarButtons.value(QString::fromNSString(identifier));

        auto item = [[NSCustomTouchBarItem alloc] initWithIdentifier:identifier];
        [item setCustomizationLabel:buttonDetails.text.toNSString()];

        auto button = [NSButton buttonWithTitle:buttonDetails.text.toNSString() target:self action:@selector(touchBarActionClicked:)];
        [button setIdentifier:identifier];

        if (!buttonDetails.icon.isNull()) {
            auto image = buttonDetails.icon.pixmap(QSize(64, 64)).toImage();
            libContemporaryCommon::tintImage(image, Qt::white);
            auto cgImage = image.toCGImage();
            auto nsimage = [[NSImage alloc] initWithCGImage:cgImage size:(NSSize){24, 24}];
            [button setImage:nsimage];
        }

        [item setView:button];
        return item;
    }
    return nil;
}

- (void)touchBarActionClicked: (NSButton*) button {
    auto identifier = [button identifier];
    auto buttonDetails = MainWindowTouchBarHelper::touchBarButtons.value(QString::fromNSString(identifier));
    buttonDetails.handler(self.mainWindow);

    [[self getVerbTouchBarItem] dismissPopover:nil];
}

- (void)installAsDelegateForWindow: (NSWindow *) window {
    _qtDelegate = window.delegate; // Save current delegate for forwarding
    window.delegate = self;
}

- (BOOL)respondsToSelector: (SEL) aSelector {
    return [_qtDelegate respondsToSelector:aSelector] || [super respondsToSelector:aSelector];
}

- (void)forwardInvocation: (NSInvocation *) anInvocation {
    [anInvocation invokeWithTarget:_qtDelegate];
}

- (NSApplicationPresentationOptions)window:(NSWindow *)window willUseFullScreenPresentationOptions:(NSApplicationPresentationOptions)proposedOptions {
    //On an unrelated note, set full screen window properties
    return (NSApplicationPresentationFullScreen | NSApplicationPresentationHideDock | NSApplicationPresentationAutoHideMenuBar | NSApplicationPresentationAutoHideToolbar);
}

@end

struct MainWindowMacPrivate {
    MainWindowTouchBarProvider *touchBarProvider;
};

void MainWindow::setupMacOs() {
    md = new MainWindowMacPrivate();

    //Disable automatic window tabbing
    [NSWindow setAllowsAutomaticWindowTabbing:NO];

    //Install TouchBarProvider as window delegate
    NSView *view = reinterpret_cast<NSView *>(this->winId());
    md->touchBarProvider = [[MainWindowTouchBarProvider alloc] init:this];
    [md->touchBarProvider installAsDelegateForWindow:view.window];
}

void MainWindow::updateContextMacOs() {
    auto verbTouchBarItem = [md->touchBarProvider getVerbTouchBarItem];
    auto sendTouchBarItem = [[md->touchBarProvider touchBar] itemForIdentifier:@"com.vicr123.thezzz.send-request"];

    auto abstractRequest = this->currentRequest();

    if (abstractRequest) {
        if (auto request = abstractRequest.dynamicCast<ZzzRequest>()) {
          [[sendTouchBarItem view] setHidden:NO];
          [[verbTouchBarItem view] setHidden:NO];

          auto verbButton = (NSButton*)[verbTouchBarItem view];
          auto icon = ZzzHelpers::iconForVerb(request->verb());
          [verbButton setTitle:request->verb().left(10).toNSString()];
          if (!icon.isNull()) {
              auto cgImage = icon.pixmap(QSize(64, 64)).toImage().toCGImage();
              auto image = [[NSImage alloc] initWithCGImage:cgImage size:(NSSize){24, 24}];
              [verbButton setImage:image];
              [verbButton setImagePosition:NSImageOnly];
          } else {
              [verbButton setImage:nil];
          }
        } else {
          [[sendTouchBarItem view] setHidden:YES];
          [[verbTouchBarItem view] setHidden:YES];
          [verbTouchBarItem dismissPopover:nil];
        }
    } else {
        [[sendTouchBarItem view] setHidden:YES];
        [[verbTouchBarItem view] setHidden:YES];
        [verbTouchBarItem dismissPopover:nil];
    }

    NSView *view = reinterpret_cast<NSView *>(this->winId());
    [view setTouchBar:nil];
}

void MainWindow::destroyMacOs() {
    delete md;
}
