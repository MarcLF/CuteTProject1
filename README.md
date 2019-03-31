# Carmelita Engine

Carmelita Engine is an Engine built in QT and C++ made by Marc López Fábregas, Adrián Castillo López and Daniel López López.

![Carmelita](https://github.com/MarcLF/CuteTProject1/blob/master/DocumentationImages/CARMELITA.PNG?raw=true)

## Features:

### Hierarchy
We have a Qlist of entities and you can add and remove them with the buttons that you can see in the picture.

![Hierarchy](https://github.com/MarcLF/CuteTProject1/blob/master/DocumentationImages/Hierarchy.PNG?raw=true)

### Inspector

We have an inspector layout where you can check out and change entities' name and components.

![Inspector](https://github.com/MarcLF/CuteTProject1/blob/master/DocumentationImages/Inspecto.PNG?raw=true)

#### Component Transform

The transform component allows you to see the position, rotation and scale of an entity and modify it dinamically.

![Transform](https://github.com/MarcLF/CuteTProject1/blob/master/DocumentationImages/Trans.PNG?raw=true)

#### Component Shape Renderer

The shape renderer component makes possible to create shapes and modify it's size, colour...

![ShapeRenderer](https://github.com/MarcLF/CuteTProject1/blob/master/DocumentationImages/SHRENDER.PNG?raw=true)

### Save and Load

The current scene can be saved and loaded through the menu bar with the open project and save project actions. It generates a Json file with all the scene data.

![Actions](https://github.com/MarcLF/CuteTProject1/blob/master/DocumentationImages/saveload.png?raw=true)
  
## QT Features

### Signals
   
- SIGNAL(valueChanged(double)) in component transform to update it everytime a value is changed.

- SIGNAL(clicked()) in the hierarchy to add and remove entities.

- SIGNAL(itemClicked(QListWidgetItem*)) in the hierarchy to select entities.

- SIGNAL(editingFinished()) in the inspector to change the name of the entity when we finished editing it's name

- SIGNAL(triggered()) in the menu bar to perfom the actions when we click the buttons.

- SIGNAL(timeout()) in the widget to paint the scene every X time.

- SIGNAL(clicked()) in the shape renderer component to open the color picker menu.

- SIGNAL(accepted()) in the shape renderer component to appply the new color to the shape.

### Slots

#### Component Transform
- void modifyXPos(double value);
- void modifyYPos(double value);
- void modifyXRot(double value);
- void modifyYRot(double value);
- void modifyXScale(double value);
- void modifyYScale(double value);

#### Hierarchy
- void AddEntity();
- void RemoveEntity();
- void SelectEntity(QListWidgetItem* item);
- std::vector<Entity*> GetEntityList();

#### Inspector
- void NameChanged();

#### Main Window
- void openProject();
- void saveProject();

#### Paint Widget
- void paintEvent(QPaintEvent *event) override;
- void myUpdate();

#### Shape Renderer Component
- void onFillColor();
- void changeFillColor();
- void onStrokeColor();
- void changeStrokeColor();

### Style Sheet

We use the style sheet named QT Dark Fusion Style.
You can find it here : https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle

