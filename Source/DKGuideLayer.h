/**
 @author Contributions from the community; see CONTRIBUTORS.md
 @date 2005-2016
 @copyright MPL2; see LICENSE.txt
*/

#import <Cocoa/Cocoa.h>
#import "DKLayer.h"

NS_ASSUME_NONNULL_BEGIN

@class DKGuide;

/** @brief Implements horizontal and vertical guidelines.
 
 @discussion A guide layer implements any number of horizontal and vertical guidelines and provides methods for snapping points and rectangles
 to them.

 A drawing typically has one guide layer, though you are not limited to just one - however since DKDrawing will generally provide
 snapping to both guides and grid as a high-level method, having more than one guide layer can create ambiguities for the user
 unless your client code takes account of the possibility of multiple guide layers.

 The default snap tolerance for guides is 6 points.

 By default guides don't snap to the grid. You can force a guide to snap to the grid even if this setting is off by dragging with
 the shift key down.
*/
@interface DKGuideLayer : DKLayer <NSCoding> {
@private
	NSMutableArray<DKGuide*>* m_hGuides; // the list of horizontal guides
	NSMutableArray<DKGuide*>* m_vGuides; // the list of vertical guides
	BOOL m_snapToGrid; // YES if snap to grid is enabled
	BOOL m_showDragInfo; // YES if dragging a guide displays the floating info window
	DKGuide* m_dragGuideRef; // the current guide being dragged
	CGFloat m_snapTolerance; // the current snap tolerance value
	NSRect mGuideDeletionZone; // guides dragged outside this rect are deleted
	BOOL mDrawGuidesInClipView; // if YES, guides are extended to be drawn in the clip view of an enclosing scroller
}

// default snapping tolerance:

/** @brief The distance a point needs to be before it is snapped to a guide
 */
@property (class) CGFloat defaultSnapTolerance;

/** @name Adding and Removing Guides
 @brief Adding and removing guides.
 @{
 */

/** @brief Adds a guide to the layer

 Sets the guide's colour to the layer's guide colour initially - after adding the guide colour can
 be set individually if desired.
 @param guide an existing guide object
 */
- (void)addGuide:(DKGuide*)guide;

/** @brief Removes a guide from the layer
 @param guide an existing guide object
 */
- (void)removeGuide:(DKGuide*)guide;

/** @brief Removes all guides permanently from the layer
 */
- (void)removeAllGuides;

/** @brief Creates a new vertical guide at the point <code>p</code>, adds it to the layer and returns it

 This is a convenient way to add a guide interactively, for example when dragging one "off" a
 ruler. See \c DKViewController for an example client of this method. If the layer is locked this
 does nothing and returns <code>nil</code>.
 @param p A point local to the drawing.
 @return The guide created, or <code>nil</code>.
 */
- (nullable DKGuide*)createVerticalGuideAndBeginDraggingFromPoint:(NSPoint)p;

/** @brief Creates a new horizontal guide at the point <code>p</code>, adds it to the layer and returns it

 This is a convenient way to add a guide interactively, for example when dragging one "off" a
 ruler. See \c DKViewController for an example client of this method. If the layer is locked this
 does nothing and returns <code>nil</code>.
 @param p A point local to the drawing.
 @return The guide created, or <code>nil</code>.
 */
- (nullable DKGuide*)createHorizontalGuideAndBeginDraggingFromPoint:(NSPoint)p;

/** @brief Get all current guides.
 */
@property (readonly, copy) NSArray<DKGuide*>* guides;

/** @brief Adds a set of guides to the elayer
 @param guides an array of guide objects
 */
- (void)setGuides:(NSArray<DKGuide*>*)guides;

/** @} */

// finding guides close to a given position

/** @brief Locates the nearest guide to the given position, if position is within the snap tolerance
 @param pos a verical coordinate value, in points
 @return the nearest guide to the given point that lies within the snap tolerance, or nil
 */
- (nullable DKGuide*)nearestVerticalGuideToPosition:(CGFloat)pos;

/** @brief Locates the nearest guide to the given position, if position is within the snap tolerance
 @param pos a horizontal coordinate value, in points
 @return the nearest guide to the given point that lies within the snap tolerance, or nil
 */
- (nullable DKGuide*)nearestHorizontalGuideToPosition:(CGFloat)pos;

/** @brief Returns the list of vertical guides.

 @discussion The guides returns are not in any particular order.
 */
@property (readonly, strong) NSArray<DKGuide*>* verticalGuides;

/** @brief Returns the list of horizontal guides.

 @discussion The guides returns are not in any particular order.
 */
@property (readonly, strong) NSArray<DKGuide*>* horizontalGuides;

// setting a common colour for the guides:

/** @brief The layer's guide colour

 @discussion Note that setting this doesn't mark the guide for update - \c DKGuideLayer has a method for doing that.
 The guide colour is actually synonymous with the "selection" colour inherited from DKLayer, but
 also each guide is able to have its own colour. This returns the selection colour, but if guides
 have their own colours this says nothing about them.
 @return a colour
 */
@property (strong) NSColor* guideColour;

// set whether guides snap to grid or not

/** @brief Whether guids should snap to the grid by default or not.

 The default is NO
 */
@property BOOL guidesSnapToGrid;

// set the snapping tolerance for this layer

/** @brief Sets the distance a point needs to be before it is snapped to a guide

 The default value is determind by the class method of the same name
 */
@property CGFloat snapTolerance;

// set whether the info window is displayed or not

/** @brief Set whether the info window should be displayed when dragging a guide

 Default is YES, display the window
 Set to \c YES to display the window, \c NO otherwise.
 */
@property BOOL showsDragInfoWindow;

/** @brief Sets a rect for which guides will be deleted if they are dragged outside of it

 Default is the same as the drawing's interior
 */
@property NSRect guideDeletionRect;

@property (nonatomic) BOOL guidesDrawnInEnclosingScrollview;

/** @name Snapping Points and Rects:
 @brief Snapping points and rects to the guides.
 @{
 */

/** @brief Snap a given point to any nearest guides within the snap tolerance

 X and y coordinates of the point are of course, individually snapped, so only one coordinate
 might be modified, as well as none or both.
 @param p a point in local drawing coordinates 
 @return a point, either the same point passed in, or a modified one that has been snapped to the guides
 */
- (NSPoint)snapPointToGuide:(NSPoint)p;

/** @brief Snaps any corner of the given rect to any nearest guides within the snap tolerance

 The rect size is never changed by this method, but its origin may be. Does not snap the centres.
 @param r a rect in local drawing coordinates 
 @return a rect, either the same rect passed in, or a modified one that has been snapped to the guides
 */
- (NSRect)snapRectToGuide:(NSRect)r;

/** @brief Snaps any corner or centre point of the given rect to any nearest guides within the snap tolerance

 The rect size is never changed by this method, but its origin may be.
 @param r a rect in local drawing coordinates 
 @param centre YES to also snap mid points of all sides, NO to just snap the corners
 @return a rect, either the same rect passed in, or a modified one that has been snapped to the guides
 */
- (NSRect)snapRectToGuide:(NSRect)r includingCentres:(BOOL)centre;

/** @brief Snaps any of a list of points to any nearest guides within the snap tolerance

 This is intended as one step in the snapping of a complex object to the guides, where points are
 arbitrarily distributed (e.g. not in a rect). Any of the points can snap to the guide - the first
 point in the list that actually snaps is used. The return value is intended to be used to offset
 a mouse point or similar so that the whole object is shifted by that amount to effect the snap.
 Note that h and v offsets are independent, and may not refer to the same actual input point.
 @param arrayOfPoints a list of NSValue object containing pointValues 
 @return a size, being the offset between whichever point was snapped and its snapped position
 */
- (NSSize)snapPointsToGuide:(NSArray<NSValue*>*)arrayOfPoints;

/** @brief Snaps any of a list of points to any nearest guides within the snap tolerance

 This is intended as one step in the snapping of a complex object to the guides, where points are
 arbitrarily distributed (e.g. not in a rect). Any of the points can snap to the guide - the first
 point in the list that actually snaps is used. The return value is intended to be used to offset
 a mouse point or similar so that the whole object is shifted by that amount to effect the snap.
 Note that h and v offsets are independent, and may not refer to the same actual input point.
 @param arrayOfPoints a list of NSValue object containing pointValues 
 @param gv if not NULL, receives the actual vertical guide snapped to
 @param gh if not NULL, receives the actual horizontal guide snapped to
 @return a size, being the offset between whichever point was snapped and its snapped position
 */
- (NSSize)snapPointsToGuide:(NSArray<NSValue*>*)arrayOfPoints verticalGuide:(DKGuide* _Nullable* _Nullable)gv horizontalGuide:(DKGuide* _Nullable* _Nullable)gh;

/** @} */
// redrawing the guides

/** @brief Marks a partiuclar guide as needing to be readrawn
 @param guide the guide to update
 */
- (void)refreshGuide:(DKGuide*)guide;

/** @brief Returns the rect occupied by a given guide

 This allows a small amount either side of the guide, and runs the full dimension of the drawing
 in the direction of the guide.
 @param guide the guide whose rect we are interested in
 @return a rect, in drawing coordinates
 */
- (NSRect)guideRect:(DKGuide*)guide;

/** @name User Actions
 @brief User actions.
 @{
 */

/** @brief High level action to remove all guides from the layer

 Can be hooked directly to a menu item for clearing the guides - will be available when the guide
 layer is active. Does nothing if the layer is locked.
 @param sender the action's sender
 */
- (IBAction)clearGuides:(id)sender;

/** @} */

@end

/** @brief each guide is implemented by an instance of DKGuide: */
@interface DKGuide : NSObject <NSCoding> {
@private
	CGFloat m_position;
	BOOL m_isVertical;
	NSColor* m_colour;
}

/** @brief Sets the position of the guide
 */
@property CGFloat guidePosition;

/** @brief Sets whether the guide is vertically oriented or horizontal
 Set to \c YES for a vertical guide, \c NO for a horizontal guide.
 */
@property BOOL isVerticalGuide;

@property (strong) NSColor* guideColour;

/** @brief Draws the guide

 Is called by the guide layer only if the guide needs to be drawn
 @param rect the update rect 
 @param lw the line width to draw
 */
- (void)drawInRect:(NSRect)rect lineWidth:(CGFloat)lw;

@end

NS_ASSUME_NONNULL_END
