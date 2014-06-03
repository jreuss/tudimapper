#include "colliderwidget.h"
#include "ui_colliderwidget.h"

/* =======================================
 * region: c/d-tor
 * =====================================*/

ColliderWidget::ColliderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColliderWidget)
{
    import=true;
    mCurrentItem = NULL;
    ui->setupUi(this);

    ui->graphicsView->setBackgroundRole(QPalette::Dark);
    ui->graphicsView->setRenderHints (QPainter::Antialiasing |
                                      QPainter::HighQualityAntialiasing);

    ui->group_meshdetail->setEnabled (false);

    model = new ColliderTreeGraphicsModel(3);
    ui->treeView_collider_prop->setModel(model);
    ui->treeView_collider_prop->setSelectionMode(QAbstractItemView::ExtendedSelection);

}

ColliderWidget::~ColliderWidget()
{
    delete ui;
}

/* =======================================
 * region: Connections
 * =====================================*/
void ColliderWidget::setupConnections()
{

    connect(ui->slider_meshdetail,SIGNAL(valueChanged(int)),
            this, SLOT(handleImportItemFixtureDetailChanged(int)));

    connect(ui->btn_box, SIGNAL(clicked()),
            this, SLOT(handleAddBoxCollider()));

    connect(ui->btn_circle, SIGNAL(clicked()),
            this, SLOT(handleAddCircleCollider()));

    connect(ui->btn_mesh,SIGNAL(clicked()),
            this, SLOT(handleAddCustomMeshCollider()));

    connect(ui->btn_contour, SIGNAL(clicked()),
            this, SLOT(handleAddContourCollider()));

    connect(ui->btn_convex, SIGNAL(clicked()),
            this, SLOT(handleAddConvexCollider()));

    if(import){
    connect(this,SIGNAL(onToggleMainUIEnabled()),
            QWidget::window (),SLOT(handleToggleMainUIEnabled()));
    }
    connect (ui->btngroup_applymeshdetail, SIGNAL(accepted()),
             this,SLOT(handleMeshDetailAccepted()));

    connect (ui->btngroup_applymeshdetail, SIGNAL(rejected()),
             this,SLOT(handleMeshDetailRejected()));

    connect (ui->btn_select, SIGNAL(toggled(bool)),
             this, SLOT(handleSelectToolSelected(bool)));

    connect (ui->btn_scale, SIGNAL(toggled(bool)),
             this, SLOT(handleScaleToolSelected(bool)));

    connect (ui->treeView_collider_prop, SIGNAL(clicked(QModelIndex)),
             this, SLOT(handleTreeviewSelectionChanged(QModelIndex)));
    connect (ui->checkBox_addToAll, SIGNAL(toggled(bool)),this,SLOT(handleAddToAllCheckbox(bool)));
}

void ColliderWidget::onLoadSelectedItem(ItemTemplate *item)
{
    if(mCurrentItem){
        //DISCONNECT THE PREVIOUS CONNECTIONSS (NOT DONE);
        ui->btn_select->setChecked(true);
    }

    mCurrentItem = item;
    if(mCurrentItem->scene()) {
        toggleToolbarButtons (All, false);

        if(mCurrentItem->contour().size() > 1){
            mMultipleObject = true;
            ui->label_addToAll->setEnabled(true);
            ui->checkBox_addToAll->setEnabled(true);
            ui->checkBox_addToAll->setChecked(true);
            handleAddToAllCheckbox(ui->checkBox_addToAll->isChecked());
        } else {
            mMultipleObject = false;
            ui->label_addToAll->setEnabled(false);
            ui->checkBox_addToAll->setEnabled(false);
            ui->btn_contour->setEnabled(true);
            ui->btn_convex->setEnabled(true);
            ui->btn_mesh->setEnabled(true);

        }

        ui->graphicsView->setScene (mCurrentItem->scene());
        model->setRoot(mCurrentItem->getColliderRoot());
        model->layoutChanged();
        connect(mCurrentItem->scene(), SIGNAL(onItemDeleted()),
                this,SLOT(handleItemRemoved()));
        connect(mCurrentItem->scene(), SIGNAL(selectionChanged()),
                this,SLOT(handleSceneSelectionChanged()));
    }
}

/* =======================================
 * region: UI toggles
 * =====================================*/

void ColliderWidget::toggleToolbarButtons(ColliderWidget::Toolbar btnGroup, bool enable)
{
    //set the transform to selection tool if the other tools are disabled
    //if(!enable)
    {
        ui->btn_select->setChecked(true);
        ui->btn_select->update();
    }
    switch(btnGroup){
    case All:
        ui->btn_scale->setEnabled (enable);
        break;
    case Transformations:
        ui->btn_scale->setEnabled (enable);
        break;
    default:
        break;
    }
}

void ColliderWidget::toggleSceneSelection(bool enable)
{
    foreach(QGraphicsItem *itm, mCurrentItem->scene ()->items ()) {
        Collider *col = static_cast<Collider *>(itm);

        //This check makes shure that it is collider and not the background or image
        //(COULD MAYBE BE DONE BETTER)
        if(col->type() == BoxType || col->type() == MeshType || col->type() == CircleType || col->type() == NodeType ){
            if(col->isSelected() == false ){
                col->setFlag (QGraphicsItem::ItemIsSelectable, enable);
                col->setFlag(QGraphicsItem::ItemIsMovable, enable);

            }
        }
    }
}

void ColliderWidget::toggleSelectionTool(bool enable)
{
    foreach(QGraphicsItem *itm, mCurrentItem->scene ()->items ()) {
        Collider *col = static_cast<Collider *>(itm);

        //This check makes shure that it is collider and not the background or image
        //(COULD MAYBE BE DONE BETTER)
        if(col->type() == BoxType || col->type() == MeshType || col->type() == CircleType){
            col->setFlag(QGraphicsItem::ItemIsMovable, enable);
            //makes shure that an object with rotate or scale is front
            col->setZValue(1);


            //This is done so that when you are on a transform tool with one object
            // and you make a new object the enabled transform of the old is set to false
            // there should be a smarter way, but with the current way of doing things this
            // this seems as the most efficient
            col->setScaleEnabled(false);
            col->setRotateEnabled(false);

        }
        if(col->type() == NodeType)
        {

            col->setFlag(QGraphicsItem::ItemIsMovable, enable);
            //BECAUSE IT IS SELeCTABLE IT CAN CAUSE ANOYANCE WHEN A NODE IS
            //ON THE SCALEORIGINPOINTS IS YOU WILL THEN SELECT A NODE RATHER
            //THAN SCALLIN
            //col->setFlag(QGraphicsItem::ItemIsSelectable, enable);
            //the same a above
            col->setZValue(1);
        }
    }
}


void ColliderWidget::toggleAddColliderWidget()
{
    ui->groupBox->setEnabled (!ui->groupBox->isEnabled ());
}

void ColliderWidget::toggleMainUI()
{
    emit onToggleMainUIEnabled ();
}

void ColliderWidget::toogleUI(bool enable)
{
    toggleAddColliderWidget ();
    toggleSceneSelection (enable);
    toggleToolbarButtons (All, enable);
    toggleMainUI ();

    ui->slider_meshdetail->setEnabled (true);
    ui->btngroup_applymeshdetail->buttons ()[0]->setEnabled (true);
}

void ColliderWidget::deselectAllItems()
{
    // deselect all items in scene
    foreach(QGraphicsItem *itm, mCurrentItem->scene ()->selectedItems ()){
        itm->setSelected (false);
    }
}

void ColliderWidget::closeWithOutFinishMesh()
{
    if(ui->btngroup_applymeshdetail->isEnabled()){
        handleMeshDetailRejected();
    }
}

/* =======================================
 * region: SLOTS
 * =====================================*/

void ColliderWidget::handleImportItemFixtureDetailChanged(int value)
{
    std::vector<cv::Point> fixture;
    if(ui->checkBox_addToAll->isChecked() || !mMultipleObject){
        for(uint i = 0; i < mCurrentItem->contour().size(); i++){
            MeshCollider *group = mNewestMeshColliders.at(i);
            group->clearChildren ();
            if(mContourFixture){
                fixture = mImgProc.decimateVerticies
                        (mCurrentItem->contour().at(i), ui->slider_meshdetail->maximum ()-value);
            } else {
                fixture = mImgProc.decimateVerticies
                        (mCurrentItem->convex().at(i), ui->slider_meshdetail->maximum ()-value);
            }
            foreach(cv::Point p, fixture){
                MeshNode *node = new MeshNode(group);
                node->setPos (p.x,p.y);
                node->setFlag (QGraphicsItem::ItemIsMovable, false);
                node->setFlag (QGraphicsItem::ItemIsSelectable, false);
                group->addChildNode (node);
            }

            group->updatePolygon ();
        }
    } else {
        MeshCollider *group = mNewestMeshColliders.back();

        group->clearChildren ();
        if(mContourFixture){
            fixture = mImgProc.decimateVerticies
                    (mCurrentItem->contour().front(), ui->slider_meshdetail->maximum ()-value);
        } else {
            fixture = mImgProc.decimateVerticies
                    (mCurrentItem->convex().front(), ui->slider_meshdetail->maximum ()-value);
        }
        foreach(cv::Point p, fixture){
            MeshNode *node = new MeshNode(group);
            node->setPos (p.x,p.y);
            node->setFlag (QGraphicsItem::ItemIsMovable, false);
            node->setFlag (QGraphicsItem::ItemIsSelectable, false);
            group->addChildNode (node);
        }

        group->updatePolygon ();
    }
}


void ColliderWidget::handleAddBoxCollider()
{
    if(ui->checkBox_addToAll->isChecked() || !mMultipleObject){
        for (uint i = 0 ; i< mCurrentItem->contour().size();i++){
            BoxCollider *box = new BoxCollider("Collider " + QString::number(mCurrentItem->getColliderRoot()->childCount()));
            QRectF rect = mImgProc.getRect ( mCurrentItem->contour().at(i));
            box->setRect (rect);
            deselectAllItems();
            box->setParentItem(mCurrentItem->getColliderRoot());
            mCurrentItem->getColliderRoot()->addChild(box);
            model->layoutChanged();
            box->setSelected (true);
        }
    } else {
            BoxCollider *box = new BoxCollider("Collider " + QString::number(mCurrentItem->getColliderRoot()->childCount()));
            QRectF rect = mImgProc.getRect ( mCurrentItem->contour().front());
            box->setRect (rect);
            deselectAllItems();
            box->setParentItem(mCurrentItem->getColliderRoot());
            mCurrentItem->getColliderRoot()->addChild(box);
            model->layoutChanged();
            box->setSelected (true);
    }

    //set the box collider to be selected and the transform tool to selection tool

    ui->btn_select->setChecked(true);
}

void ColliderWidget::handleAddCircleCollider()
{
    if(ui->checkBox_addToAll->isChecked() || !mMultipleObject){
        for (uint i = 0 ; i< mCurrentItem->contour().size();i++){
            CircleCollider *circle = new CircleCollider("Collider " + QString::number(mCurrentItem->getColliderRoot()->childCount()));
            cv::Point2f center;
            float radius;
            cv::minEnclosingCircle( cv::Mat(mCurrentItem->contour().at(i)), center, radius);
            circle->setCenter(QPointF(center.x,center.y));
            circle->setRadius(radius);
            deselectAllItems();
            circle->setParentItem(mCurrentItem->getColliderRoot());
            mCurrentItem->getColliderRoot()->addChild(circle);
            model->layoutChanged();

            circle->setSelected (true);
        }
    } else {
        CircleCollider *circle = new CircleCollider("Collider " + QString::number(mCurrentItem->getColliderRoot()->childCount()));
        cv::Point2f center;
        float radius;
        cv::minEnclosingCircle( cv::Mat(mCurrentItem->contour().front()), center, radius);
        circle->setCenter(QPointF(center.x,center.y));
        circle->setRadius(radius);
        deselectAllItems();
        circle->setParentItem(mCurrentItem->getColliderRoot());
        mCurrentItem->getColliderRoot()->addChild(circle);
        model->layoutChanged();

    }
    ui->btn_select->setChecked(true);
}

void ColliderWidget::handleAddContourCollider()
{
    {
        // enable mesh detail widget
         ui->treeView_collider_prop->setEnabled(false);
        ui->group_meshdetail->setEnabled (true);
        onNonAcceptedMesh = true;
        mNewestMeshColliders.clear();
        // disable all other ui elements
        toogleUI (false);
        if(ui->checkBox_addToAll->isChecked() || !mMultipleObject){
            deselectAllItems();
            for(uint i = 0; i < mCurrentItem->contour().size(); i++){
                MeshCollider *group = new MeshCollider("Collider " + QString::number(mCurrentItem->getColliderRoot()->childCount()),true);
                group->setSize (QSize(mCurrentItem->image ().width (),
                                      mCurrentItem->image ().height ()));
                group->setParentItem(mCurrentItem->getColliderRoot());
                mCurrentItem->getColliderRoot()->addChild(group);
                group->setSelected (true);
                mNewestMeshColliders.append(group);

            }
            toggleSceneSelection(false);
            mContourFixture = true;

            handleImportItemFixtureDetailChanged(4);
            model->layoutChanged();
        } else {
            MeshCollider *group = new MeshCollider("Collider " + QString::number(mCurrentItem->getColliderRoot()->childCount()),true);

            group->setSize (QSize(mCurrentItem->image ().width (),
                                  mCurrentItem->image ().height ()));


            deselectAllItems();

            mNewestMeshColliders.append(group);
            group->setParentItem(mCurrentItem->getColliderRoot());
            mCurrentItem->getColliderRoot()->addChild(group);
            model->layoutChanged();

            group->setSelected (true);

            toggleSceneSelection(false);
            mContourFixture = true;

            handleImportItemFixtureDetailChanged(4);
        }
    }
}

void ColliderWidget::handleAddConvexCollider()
{
    // enable mesh detail widget
    ui->treeView_collider_prop->setEnabled(false);
    ui->group_meshdetail->setEnabled (true);
    onNonAcceptedMesh = true;
    mNewestMeshColliders.clear();
    // disable all other ui elements
    toogleUI (false);
    if(ui->checkBox_addToAll->isChecked()){
        deselectAllItems();
        for(uint i = 0; i < mCurrentItem->convex().size(); i++){
            MeshCollider *group = new MeshCollider("Collider " + QString::number(mCurrentItem->getColliderRoot()->childCount()),true);
            group->setSize (QSize(mCurrentItem->image ().width (),
                                  mCurrentItem->image ().height ()));
            group->setParentItem(mCurrentItem->getColliderRoot());
            mCurrentItem->getColliderRoot()->addChild(group);
            group->setSelected (true);
            mNewestMeshColliders.append(group);

        }
        toggleSceneSelection(false);
        mContourFixture = false;

        handleImportItemFixtureDetailChanged(4);
        model->layoutChanged();
    } else {
        MeshCollider *group = new MeshCollider("Collider " + QString::number(mCurrentItem->getColliderRoot()->childCount()),true);

        group->setSize (QSize(mCurrentItem->image ().width (),
                              mCurrentItem->image ().height ()));


        deselectAllItems();

        mNewestMeshColliders.append(group);
        group->setParentItem(mCurrentItem->getColliderRoot());
        mCurrentItem->getColliderRoot()->addChild(group);
        model->layoutChanged();

        group->setSelected (true);

        toggleSceneSelection(false);
        mContourFixture = false;

        handleImportItemFixtureDetailChanged(4);
    }
}

void ColliderWidget::handleAddCustomMeshCollider()
{
    // enable mesh detail widget
    toogleUI (false);
     ui->treeView_collider_prop->setEnabled(false);
    onNonAcceptedMesh = true;

    ui->group_meshdetail->setEnabled (true);
    /* the slider is not used when drawing
     * custom meshes */
    ui->slider_meshdetail->setEnabled (true);
    ui->btngroup_applymeshdetail->buttons ()[0]->setEnabled (false);


    // set timed event to check collider validity (:point > 2)
    connect (&mTimer, SIGNAL(timeout()),
             this, SLOT(handleCheckColliderOnTimerShot()));
    mTimer.setInterval (500);
    mTimer.start ();

    mNewestMeshColliders.clear();
    MeshCollider *group = new MeshCollider("Collider " + QString::number(mCurrentItem->getColliderRoot()->childCount()), false);

    group->setSize (QSize(mCurrentItem->image ().width (),
                          mCurrentItem->image ().height ()));


    // deselect all items in scene
    mCurrentItem->scene ()->selectedItems ().clear();
    group->setSelected (true);

    mNewestMeshColliders.append(group);
    group->setParentItem(mCurrentItem->getColliderRoot());
    mCurrentItem->getColliderRoot()->addChild(group);
    model->layoutChanged();
}

void ColliderWidget::handleMeshDetailAccepted()
{
    // enable all other ui elements
    toogleUI (true);
     ui->treeView_collider_prop->setEnabled(true);
    onNonAcceptedMesh = false;
    if(mTimer.isActive ()) {
        mTimer.stop ();
        disconnect (&mTimer, SIGNAL(timeout()),
                    this, SLOT(handleCheckColliderOnTimerShot()));
    }

    foreach(MeshCollider* col, mNewestMeshColliders){
        col->setConsolidated ();
        col->setCanBeDeleted(true);
        col->setFlag (QGraphicsItem::ItemIsMovable, true);
        col->updatePolygon ();
    }

    ui->group_meshdetail->setEnabled (false);
    //set the transform tool to selection tool
    ui->btn_select->setChecked(true);
}


void ColliderWidget::handleMeshDetailRejected()
{
    // enable all other ui elements
    toogleUI (true);
     ui->treeView_collider_prop->setEnabled(true);
    onNonAcceptedMesh = false;

    if(mTimer.isActive ()) {
        mTimer.stop ();
        disconnect (&mTimer, SIGNAL(timeout()),
                    this, SLOT(handleCheckColliderOnTimerShot()));
    }
    foreach(MeshCollider* col, mNewestMeshColliders){
        delete col;
    }
    // disable mesh detail widget
    ui->group_meshdetail->setEnabled (false);
    model->layoutChanged();
}

void ColliderWidget::handleSceneSelectionChanged()
{   ui->treeView_collider_prop->selectionModel()->clearSelection();
    foreach(QGraphicsItem *it, mCurrentItem->scene()->selectedItems())
    {   if(it->type() != NodeType){
            AbstractTreeGraphicsItem *itm = static_cast<AbstractTreeGraphicsItem *>(it);
            QModelIndex index = model->indexFromItem(itm, static_cast<AbstractTreeGraphicsItem *>(model->getRoot()));
            ui->treeView_collider_prop->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
        }
    }
    //model->layoutChanged();

    if(mCurrentItem->scene()->selectedItems ().count() == 1 )
    {
        Collider *col = static_cast<Collider *>(mCurrentItem->scene ()->selectedItems ().front ());

        if(col->type () == BoxType || col->type () == CircleType){
            // enable transformations
            toggleToolbarButtons (Transformations, true);
            toggleToolbarButtons (Mesh, false);
        }
        else if(col->type () == MeshType){
            // enable all tool buttons
            if(!onNonAcceptedMesh) {
                toggleToolbarButtons (Transformations, true);
                toggleToolbarButtons (Mesh, true);
            }
        } else {
            toggleToolbarButtons (All, false);
        }
    } else {
        toggleToolbarButtons (All, false);
    }
}

void ColliderWidget::handleTreeviewSelectionChanged(QModelIndex pressedOn)
{
    Q_UNUSED(pressedOn);

    QModelIndexList idxList = ui->treeView_collider_prop->selectionModel()->selectedIndexes();
    deselectAllItems();
    foreach (QModelIndex idx , idxList) {
        model->itemFromIndex(idx)->setSelected(true);
    }
}

void ColliderWidget::handleAddToAllCheckbox(bool enabled)
{
    if(enabled){
        ui->btn_contour->setEnabled(true);
        ui->btn_convex->setEnabled(true);
        ui->btn_mesh->setEnabled(false);
    } else {
        ui->btn_contour->setEnabled(false);
        ui->btn_convex->setEnabled(false);
        ui->btn_mesh->setEnabled(true);
    }
}

void ColliderWidget::handleSelectToolSelected(bool selected)
{
    toggleSelectionTool(selected);
}

void ColliderWidget::handleRotateToolSelected(bool selected)
{
    if(mCurrentItem->scene()->selectedItems().count()>0){
        Collider *col =
                static_cast<Collider *>(mCurrentItem->scene ()->selectedItems ().front());
        col->setRotateEnabled (selected);
        col->setZValue(2);
        col->update();
    }
}

void ColliderWidget::handleScaleToolSelected(bool selected)
{
    if(mCurrentItem->scene()->selectedItems().count()>0){
        if(mCurrentItem->scene ()->selectedItems ().front()->type() !=NodeType){
            Collider *col =
                    static_cast<Collider *>(mCurrentItem->scene ()->selectedItems ().front());

            col->setScaleEnabled (selected);
            col->setZValue(2);
            col->update();
        }
    }
}

void ColliderWidget::handleItemRemoved()
{
    model->layoutChanged();
}

void ColliderWidget::handleCheckColliderOnTimerShot()
{
    if(mNewestMeshColliders.back()->childCount () > 2) {
        ui->btngroup_applymeshdetail->buttons ()[0]->setEnabled (true);
    }
}


