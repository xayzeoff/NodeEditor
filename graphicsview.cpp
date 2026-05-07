#include "graphicsview.h"
#include <QDebug>
#include <QMimeData>
#include <QMouseEvent>
#include <QGraphicsLineItem>
#include "node.h"
#include "ifelsewidget.h"
#include <QLabel>
#include <QGraphicsEffect>
#include <lineitem.h>
#include "semantictree.hpp"
#include <stack>
#include <QFile>
#include <QXmlStreamReader>
#include <QMessageBox>

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    setAcceptDrops(true);

    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QBrush(QColor(200,200,200),Qt::Dense7Pattern));
    setSceneRect(-5000, -5000, 10000, 10000);
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    qDebug() << "GraphicsView created";
}

void GraphicsView::dropEvent(QDropEvent *event)
{
    qDebug() << "drop";

    if (!event->mimeData()->hasText()){
        return;
    }

    QString type = event->mimeData()->text();
    qDebug() << "Type:" << type;

    QString iconPath = QString::fromUtf8(event->mimeData()->data("application/x-icon-path"));
    QPixmap icon(iconPath);

    Node *node = nullptr;
    QGraphicsProxyWidget* proxy;

    if (type == "radio") {
        node = new Node(Node::Action, icon);
        IfElseWidget* widget = new IfElseWidget;
        proxy = scene->addWidget(widget);
        proxy->setParentItem(node);
        proxy->setPos(10, 45);
        widget->setFixedWidth(240);

        node->setWdg(widget);

        Connector* left = new Connector(node);
        left->setPos(0, 55);
        left->setRight(false);
        node->addConnector(left);
        left->setNode(node);

        Connector* right = new Connector(node);
        right->setPos(260, 55);
        right->setRight(true);
        node->addConnector(right);
        right->setNode(node);
        connect(left,&Connector::drawLineSignal, this,&GraphicsView::drawLineSlot);
        connect(right,&Connector::drawLineSignal, this,&GraphicsView::drawLineSlot);

        Connector* rightFalse = new Connector(node);
        rightFalse->setPos(260, 85);
        rightFalse->setRight(true);
        node->addConnector(rightFalse);
        rightFalse->setNode(node);
        connect(rightFalse,&Connector::drawLineSignal, this,&GraphicsView::drawLineSlot);

        connect(right,&Connector::deleteNodeSignal,this, &GraphicsView::removeNodeFromConnectorSlot);
        connect(left,&Connector::deleteNodeSignal,this, &GraphicsView::removeNodeFromConnectorSlot);
        connect(rightFalse,&Connector::deleteNodeSignal,this, &GraphicsView::removeNodeFromConnectorSlot);

        connect(right,&Connector::reConnectSignal,this, &GraphicsView::reConnectFromConnectorSlot);
        connect(left,&Connector::reConnectSignal,this, &GraphicsView::reConnectFromConnectorSlot);
        connect(rightFalse,&Connector::reConnectSignal,this, &GraphicsView::reConnectFromConnectorSlot);
    }
    else if (type == "text") {
        node = new Node(Node::Simple, icon);
    } else if(type == "start"){
        node = new Node(Node::Start, icon);
        Connector* right = new Connector(node);
        right->setPos(80, 20);
        right->setRight(true);
        node->addConnector(right);
        right->setNode(node);
        connect(right,&Connector::drawLineSignal, this,&GraphicsView::drawLineSlot);
        connect(right,&Connector::deleteNodeSignal,this, &GraphicsView::removeNodeFromConnectorSlot);
        connect(right,&Connector::reConnectSignal,this, &GraphicsView::reConnectFromConnectorSlot);
        startNode = node;
    } else if(type == "textPage"){
        node = new Node(Node::TextPage, icon);
        Connector* right = new Connector(node);
        right->setPos(200, 45);
        right->setRight(true);
        node->addConnector(right);
        right->setNode(node);
        connect(right,&Connector::drawLineSignal, this,&GraphicsView::drawLineSlot);
        connect(right,&Connector::deleteNodeSignal,this, &GraphicsView::removeNodeFromConnectorSlot);

        Connector* left = new Connector(node);
        left->setPos(0, 45);
        left->setRight(false);
        node->addConnector(left);
        left->setNode(node);

        connect(left,&Connector::drawLineSignal, this,&GraphicsView::drawLineSlot);
        connect(left,&Connector::deleteNodeSignal,this, &GraphicsView::removeNodeFromConnectorSlot);

        connect(right,&Connector::reConnectSignal,this, &GraphicsView::reConnectFromConnectorSlot);
        connect(left,&Connector::reConnectSignal,this, &GraphicsView::reConnectFromConnectorSlot);
    }
    else if(type == "oneOfMany"){
        node = new Node(Node::oneOfMany, icon);
        Connector* right = new Connector(node);
        right->setPos(200, 45);
        right->setRight(true);
        node->addConnector(right);
        right->setNode(node);
        connect(right,&Connector::drawLineSignal, this,&GraphicsView::drawLineSlot);

        Connector* left = new Connector(node);
        left->setPos(0, 45);
        left->setRight(false);
        node->addConnector(left);
        left->setNode(node);

        connect(left,&Connector::drawLineSignal, this,&GraphicsView::drawLineSlot);

        connect(right,&Connector::deleteNodeSignal,this, &GraphicsView::removeNodeFromConnectorSlot);
        connect(left,&Connector::deleteNodeSignal,this, &GraphicsView::removeNodeFromConnectorSlot);

        connect(right,&Connector::reConnectSignal,this, &GraphicsView::reConnectFromConnectorSlot);
        connect(left,&Connector::reConnectSignal,this, &GraphicsView::reConnectFromConnectorSlot);
        numQuestion ++;
        node->setName("Вопрос "+QString::number(numQuestion));
    }
    else if(type == "server"){
        node = new Node(Node::Server, icon);
        Connector* right = new Connector(node);
        Connector* left = new Connector(node);
        right->setPos(120, 20);
        right->setRight(true);

        left->setPos(0, 20);
        left->setRight(false);

        node->addConnector(right);
        node->addConnector(left);

        right->setNode(node);
        left->setNode(node);



        connect(right,&Connector::drawLineSignal, this,&GraphicsView::drawLineSlot);
        connect(right,&Connector::deleteNodeSignal,this, &GraphicsView::removeNodeFromConnectorSlot);
        connect(right,&Connector::reConnectSignal,this, &GraphicsView::reConnectFromConnectorSlot);

        connect(left,&Connector::drawLineSignal, this,&GraphicsView::drawLineSlot);
        connect(left,&Connector::deleteNodeSignal,this, &GraphicsView::removeNodeFromConnectorSlot);
        connect(left,&Connector::reConnectSignal,this, &GraphicsView::reConnectFromConnectorSlot);
    }
    else {
        node = new Node(Node::Random, icon);
    }

    QPointF scenePos = mapToScene(event->position().toPoint());
    scene->addItem(node);
    nodes.push_back(node);
    node->setPos(scenePos);

    connect(node,&Node::moveSignal, this,&GraphicsView::reDrawLineSlot);
    connect(node,&Node::pressSignal, this,&GraphicsView::changeCurrentPageSlot);

    qDebug() << "Node pos:" << node->scenePos();
    event->acceptProposedAction();
}

void GraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText()) {
        event->acceptProposedAction();
        qDebug() << "drag enter";
    }
}

void GraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "press" << spacePressed;
    if(event->button() == Qt::LeftButton && spacePressed){
        lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    else if (event->button() == Qt::LeftButton) {
        QGraphicsItem* itemUnder = itemAt(event->pos());
        Node* clickedNode = dynamic_cast<Node*>(itemUnder);
        if (!clickedNode) {
            if (itemUnder) {
                QGraphicsItem* parent = itemUnder->parentItem();
                while (parent) {
                    clickedNode = dynamic_cast<Node*>(parent);
                    if (clickedNode) break;
                    parent = parent->parentItem();
                }
            }
        }

        if (clickedNode && clickedNode->isSelected()) {
            m_nodeDragging    = true;
            m_dragStartScene  = mapToScene(event->pos());
            m_dragStartPositions.clear();
            for (Node* n : nodes) {
                if (n->isSelected()) {
                    m_dragStartPositions[n] = n->pos();
                    n->setFlag(QGraphicsItem::ItemIsMovable, false);
                }
            }
        } else {
            m_nodeDragging = false;
        }

        if (!itemUnder && !activeConnector) {
            m_selecting    = true;
            m_selectOrigin = mapToScene(event->pos());

            m_selectRect = new QGraphicsRectItem();

            // -------------------------------------------------------------
            QColor fillColor(59, 130, 246, 40);   // rgba(59,130,246,0.16)
            QColor strokeColor(59, 130, 246, 200); // rgba(59,130,246,0.78)
            m_selectRect->setBrush(QBrush(fillColor));
            QPen pen(strokeColor, 1.5, Qt::SolidLine);
            m_selectRect->setPen(pen);
            m_selectRect->setZValue(1000);
            m_selectRect->setRect(QRectF(m_selectOrigin, QSizeF(0, 0)));
            // -------------------------------------------------------------
            scene->addItem(m_selectRect);
        }
    }

    changeCurrentPageSlot("", nullptr);
    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::clearScene()
{
    scene->clear();
    nodes.clear();
    cLines.clear();
    startNode = nullptr;
    numQuestion = 0;
    m_currentFilePath.clear();
}


void GraphicsView::generateForm(QString path)
{
    if (!startNode) {
        return;
    }

    QMap<Node*, int> nodeId;
    QList<Node*> ordered;

    std::stack<Node*> stk;
    QSet<Node*> visited;

    // Стартовый нод — id 0
    QList<Node*> firstList = getNextNode(startNode);
    for (Node* n : firstList) {
        if (n && !visited.contains(n)) {
            stk.push(n);
        }
    }

    int idCounter = 0;
    while (!stk.empty()) {
        Node* cur = stk.top(); stk.pop();
        if (visited.contains(cur)) continue;
        visited.insert(cur);
        nodeId[cur] = idCounter++;
        ordered.append(cur);

        QList<Node*> nexts = getNextNode(cur);
        for (int i = nexts.size() - 1; i >= 0; --i) {
            if (nexts[i] && !visited.contains(nexts[i]))
                stk.push(nexts[i]);
        }
    }

    QMap<Node*, int> nextTrue;
    QMap<Node*, int> nextFalse;

    for (Node* n : ordered) {
        QList<Node*> nexts = getNextNode(n);
        nextTrue[n]  = -1;
        nextFalse[n] = -1;

        if (n->getNodetype() == Node::Action) {
            if (nexts.size() >= 1 && nexts[0] && nodeId.contains(nexts[0]))
                nextTrue[n]  = nodeId[nexts[0]];
            if (nexts.size() >= 2 && nexts[1] && nodeId.contains(nexts[1]))
                nextFalse[n] = nodeId[nexts[1]];
        } else {
            if (!nexts.isEmpty() && nexts[0] && nodeId.contains(nexts[0]))
                nextTrue[n] = nodeId[nexts[0]];
        }
    }

    QString css = generateCSS();
    QString js  = generateJS(ordered, nodeId, nextTrue, nextFalse);
    QString body = generateBody(ordered, nodeId);

    htmlCode  = "<!DOCTYPE html>\n";
    htmlCode += "<html lang=\"ru\">\n";
    htmlCode += "<head>\n";
    htmlCode += "  <meta charset=\"UTF-8\">\n";
    htmlCode += "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    htmlCode += "  <title>Опрос</title>\n";
    htmlCode += css;
    htmlCode += "</head>\n";
    htmlCode += "<body>\n";
    htmlCode += body;
    htmlCode += js;
    htmlCode += "</body>\n</html>\n";

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file:" << path;
        return;
    }
    file.write(htmlCode.toUtf8());
    file.close();
    qDebug() << "Saved to" << path;
}


void GraphicsView::addHtmlCode(TreeNode *tn)
{
    if (tn == nullptr) return;
    if (tn->nodeType == Node::TextPage) {
        qDebug() << tn->nodePointer->settings().text;
        htmlCode += " pages[0] = {'name':" + tn->nodePointer->settings().text
                    + ", 'button': 'next', 'next_id':1};";
    }
    addHtmlCode(tn->next1);
    addHtmlCode(tn->next2);
}


QString GraphicsView::generateCSS()
{
    return R"(  <style>
    *, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }

    body {
      min-height: 100vh;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      font-family: 'Segoe UI', system-ui, -apple-system, sans-serif;
      display: flex;
      align-items: center;
      justify-content: center;
      padding: 20px;
    }

    /* Обёртка всего опросника */
    .survey-wrapper {
      width: 100%;
      max-width: 680px;
    }

    /* Прогресс-бар */
    .progress-bar-outer {
      background: rgba(255,255,255,0.25);
      border-radius: 999px;
      height: 6px;
      margin-bottom: 24px;
      overflow: hidden;
    }
    .progress-bar-inner {
      background: #ffffff;
      height: 100%;
      border-radius: 999px;
      transition: width 0.4s ease;
      width: 0%;
    }

    /* Карточка страницы */
    .page {
      display: none;
      background: #ffffff;
      border-radius: 20px;
      padding: 48px 52px;
      box-shadow: 0 25px 60px rgba(0,0,0,0.18);
      animation: fadeIn 0.35s ease;
    }
    .page.active { display: block; }

    @keyframes fadeIn {
      from { opacity: 0; transform: translateY(16px); }
      to   { opacity: 1; transform: translateY(0);    }
    }

    /* Бейдж типа страницы */
    .page-badge {
      display: inline-block;
      font-size: 11px;
      font-weight: 700;
      letter-spacing: 1px;
      text-transform: uppercase;
      color: #7c3aed;
      background: #ede9fe;
      border-radius: 999px;
      padding: 4px 12px;
      margin-bottom: 16px;
    }

    /* Заголовок */
    .page-title {
      font-size: 26px;
      font-weight: 700;
      color: #1e1b4b;
      margin-bottom: 14px;
      line-height: 1.3;
    }

    /* Основной текст */
    .page-text {
      font-size: 16px;
      color: #4b5563;
      line-height: 1.7;
      margin-bottom: 32px;
      white-space: pre-wrap;
    }

    /* ----- Варианты ответов ----- */
    .options-list {
      list-style: none;
      display: flex;
      flex-direction: column;
      gap: 12px;
      margin-bottom: 32px;
    }
    .option-item label {
      display: flex;
      align-items: center;
      gap: 14px;
      padding: 16px 20px;
      border: 2px solid #e5e7eb;
      border-radius: 12px;
      cursor: pointer;
      transition: border-color 0.2s, background 0.2s;
      font-size: 15px;
      color: #374151;
    }
    .option-item label:hover {
      border-color: #a78bfa;
      background: #faf5ff;
    }
    .option-item input[type="radio"] {
      appearance: none;
      width: 20px; height: 20px;
      border: 2px solid #d1d5db;
      border-radius: 50%;
      flex-shrink: 0;
      transition: border-color 0.2s;
      position: relative;
    }
    .option-item input[type="radio"]::after {
      content: '';
      position: absolute;
      width: 10px; height: 10px;
      background: #7c3aed;
      border-radius: 50%;
      top: 50%; left: 50%;
      transform: translate(-50%,-50%) scale(0);
      transition: transform 0.15s;
    }
    .option-item input[type="radio"]:checked { border-color: #7c3aed; }
    .option-item input[type="radio"]:checked::after { transform: translate(-50%,-50%) scale(1); }
    .option-item:has(input:checked) label {
      border-color: #7c3aed;
      background: #faf5ff;
    }

    /* ----- Сообщение о правильности ответа ----- */
    .answer-feedback {
      display: none;
      padding: 12px 18px;
      border-radius: 10px;
      font-size: 14px;
      font-weight: 600;
      margin-bottom: 20px;
    }
    .answer-feedback.correct   { background: #dcfce7; color: #166534; }
    .answer-feedback.incorrect { background: #fee2e2; color: #991b1b; }

    /* ----- Кнопки ----- */
    .btn-row {
      display: flex;
      gap: 12px;
      flex-wrap: wrap;
    }
    .btn {
      display: inline-flex;
      align-items: center;
      gap: 8px;
      padding: 14px 28px;
      border: none;
      border-radius: 12px;
      font-size: 15px;
      font-weight: 600;
      cursor: pointer;
      transition: transform 0.15s, box-shadow 0.15s;
    }
    .btn:active { transform: scale(0.97); }
    .btn-primary {
      background: linear-gradient(135deg, #7c3aed, #4f46e5);
      color: #fff;
      box-shadow: 0 4px 14px rgba(124,58,237,0.35);
    }
    .btn-primary:hover { box-shadow: 0 6px 20px rgba(124,58,237,0.45); }
    .btn-secondary {
      background: #f3f4f6;
      color: #374151;
    }
    .btn-secondary:hover { background: #e5e7eb; }

    /* ----- Страница результатов ----- */
    .result-icon { font-size: 56px; text-align: center; margin-bottom: 16px; }
    .result-score {
      text-align: center;
      font-size: 48px;
      font-weight: 800;
      color: #7c3aed;
      margin-bottom: 8px;
    }
    .result-label {
      text-align: center;
      font-size: 16px;
      color: #6b7280;
      margin-bottom: 32px;
    }

    /* ----- Разделитель ----- */
    .divider { border: none; border-top: 1px solid #f3f4f6; margin: 28px 0; }
  </style>
)";
}


QString GraphicsView::generateJS(
    const QList<Node*>& ordered,
    const QMap<Node*, int>& nodeId,
    const QMap<Node*, int>& nextTrue,
    const QMap<Node*, int>& nextFalse)
{
    int total = ordered.size();

    QString pagesJS = "  var PAGES = [\n";
    for (Node* n : ordered) {
        int id = nodeId[n];
        Node::Settings st = n->settings();
        Node::Type t = n->getNodetype();
        int nt = nextTrue.value(n, -1);
        int nf = nextFalse.value(n, -1);


        QString condQuestion = "";
        QString condOp = "==";
        QString condValue = "";
        if (t == Node::Action && n->getWdg()) {
            IfElseWidget* w = n->getWdg();
            condQuestion = w->getInputBoxText();
            condOp       = w->getOperatorText();
            condValue    = w->getValueText();
        }

        pagesJS += QString(
                       "    {id:%1, type:'%2', nextTrue:%3, nextFalse:%4,"
                       " condQ:'%5', condOp:'%6', condV:'%7',"
                       " q1:'%8', q2:'%9', q3:'%10', q4:'%11', isTest:%12, correctAnswer:'%13',"
                       " serverSave:%14, localSave:%15, url:'%16', localPath:'%17', format:'%18'},\n")
                       .arg(id)
                       .arg(typeToString(t))
                       .arg(nt)
                       .arg(nf)
                       .arg(condQuestion.replace("'", "\\'"))
                       .arg(condOp)
                       .arg(condValue.replace("'", "\\'"))
                       .arg(st.q1.replace("'", "\\'"))
                       .arg(st.q2.replace("'", "\\'"))
                       .arg(st.q3.replace("'", "\\'"))
                       .arg(st.q4.replace("'", "\\'"))
                       .arg(st.test ? "true" : "false")
                       .arg(st.correctAnswer.replace("'", "\\'"))
                       .arg(st.serverSave ? "true" : "false")
                       .arg(st.localSave  ? "true" : "false")
                       .arg(st.url.replace("'", "\\'"))
                       .arg(st.path.replace("'", "\\'"))
                       .arg(st.format.replace("'", "\\'"));
    }
    pagesJS += "  ];\n";

    QString js = "\n<script>\n";
    js += "  var answers = {};\n";
    js += "  var correctCount = 0;\n";
    js += "  var totalQuestions = 0;\n";
    js += QString("  var TOTAL_PAGES = %1;\n").arg(total);
    js += pagesJS;

    js += R"(
  function showPage(id) {
    var pages = document.querySelectorAll('.page');
    pages.forEach(function(p){ p.classList.remove('active'); });

    var target = document.getElementById('page-' + id);
    if (!target) {
      // Показываем финальный результат
      showResult();
      return;
    }
    target.classList.add('active');

    var bar = document.getElementById('progress-bar');
    if (bar) bar.style.width = (((id + 1) / TOTAL_PAGES) * 100) + '%';

    var meta = PAGES[id];
    if (meta && meta.type === 'action') {
      var answer = answers[meta.condQ] || '';
      var go = -1;
      if      (meta.condOp === '==' && answer == meta.condV) go = meta.nextTrue;
      else if (meta.condOp === '!=' && answer != meta.condV) go = meta.nextTrue;
      else if (meta.condOp === '>'  && parseFloat(answer) > parseFloat(meta.condV))  go = meta.nextTrue;
      else if (meta.condOp === '<'  && parseFloat(answer) < parseFloat(meta.condV))  go = meta.nextTrue;
      else if (meta.condOp === '>=' && parseFloat(answer) >= parseFloat(meta.condV)) go = meta.nextTrue;
      else if (meta.condOp === '<=' && parseFloat(answer) <= parseFloat(meta.condV)) go = meta.nextTrue;
      else go = meta.nextFalse;

      if (go >= 0) showPage(go);
      else showResult();
      return;
    }

    if (meta && meta.type === 'server') {
      submitAnswers(meta, function() {
        if (meta.nextTrue >= 0) showPage(meta.nextTrue);
        else showResult();
      });
      return;
    }
  }

  function nextPage(currentId) {
    var meta = PAGES[currentId];
    if (!meta) return;

    if (meta.type === 'oneofmany') {
      var radios = document.querySelectorAll('#page-' + currentId + ' input[type="radio"]');
      var selected = null;
      radios.forEach(function(r){ if (r.checked) selected = r.value; });

      if (!selected) {
        var fb = document.getElementById('fb-' + currentId);
        if (fb) {
          fb.textContent = 'Пожалуйста, выберите ответ.';
          fb.className = 'answer-feedback incorrect';
          fb.style.display = 'block';
        }
        return;
      }

      var pageName = document.getElementById('title-' + currentId);
      var qName = pageName ? pageName.dataset.name : ('q' + currentId);
      answers[qName] = selected;

      if (meta.isTest) {
        totalQuestions++;
        var fb = document.getElementById('fb-' + currentId);
        if (selected === meta.correctAnswer) {
          correctCount++;
          if (fb) { fb.textContent = '✓ Верно!'; fb.className = 'answer-feedback correct'; fb.style.display = 'block'; }
        } else {
          if (fb) { fb.textContent = '✗ Неверно. Правильный ответ: ' + meta.correctAnswer; fb.className = 'answer-feedback incorrect'; fb.style.display = 'block'; }
        }

        setTimeout(function(){ goNext(currentId, meta); }, 900);
        return;
      }
    }

    goNext(currentId, meta);
  }

  function goNext(currentId, meta) {
    if (meta.nextTrue >= 0) showPage(meta.nextTrue);
    else showResult();
  }

  function prevPage(targetId) {
    showPage(targetId);
  }

  function submitAnswers(meta, callback) {
    // Формируем данные из объекта answers
    var keys = Object.keys(answers);
    var rows = ['Вопрос,Ответ'];
    keys.forEach(function(k){ rows.push(k + ',' + answers[k]); });
    var csvContent = rows.join('\n');

    var done = 0;
    var total = (meta.serverSave ? 1 : 0) + (meta.localSave ? 1 : 0);
    if (total === 0) { callback(); return; }

    function checkDone() { done++; if (done >= total) callback(); }

    if (meta.serverSave && meta.url) {
      var formData = new FormData();
      keys.forEach(function(k){ formData.append(k, answers[k]); });
      fetch(meta.url, { method: 'POST', body: formData })
        .then(function(){ checkDone(); })
        .catch(function(){ checkDone(); });
    }

    if (meta.localSave) {
      // Скачать CSV в браузере (т.к. реальное сохранение на диск невозможно из браузера)
      var blob = new Blob([csvContent], { type: 'text/csv;charset=utf-8;' });
      var a = document.createElement('a');
      a.href = URL.createObjectURL(blob);
      var fname = meta.localPath ? meta.localPath.split(/[\\/]/).pop() : 'answers.csv';
      a.download = fname || 'answers.csv';
      document.body.appendChild(a);
      a.click();
      document.body.removeChild(a);
      checkDone();
    }
  }

  function showResult() {
    var pages = document.querySelectorAll('.page');
    pages.forEach(function(p){ p.classList.remove('active'); });
    var res = document.getElementById('result-page');
    if (res) {
      res.classList.add('active');
      var bar = document.getElementById('progress-bar');
      if (bar) bar.style.width = '100%';
      var scoreEl = document.getElementById('result-score');
      if (scoreEl && totalQuestions > 0)
        scoreEl.textContent = correctCount + ' / ' + totalQuestions;
      else if (scoreEl)
        scoreEl.style.display = 'none';
    }
  }
  document.addEventListener('DOMContentLoaded', function(){
    if (TOTAL_PAGES > 0) showPage(0);
  });
</script>
)";
    return js;
}

QString GraphicsView::generateBody(
    const QList<Node*>& ordered,
    const QMap<Node*, int>& nodeId)
{
    QString html;
    html += "  <div class=\"survey-wrapper\">\n";
    html += "    <div class=\"progress-bar-outer\"><div class=\"progress-bar-inner\" id=\"progress-bar\"></div></div>\n";

    for (Node* n : ordered) {
        int id = nodeId[n];
        Node::Settings st = n->settings();
        Node::Type t = n->getNodetype();

        html += QString("    <div class=\"page\" id=\"page-%1\">\n").arg(id);

        // ---- TextPage ----
        if (t == Node::TextPage) {
            html += "      <div class=\"page-badge\">Информация</div>\n";
            QString title = st.name.isEmpty() ? "Страница" : st.name.toHtmlEscaped();
            html += QString("      <h2 class=\"page-title\" id=\"title-%1\" data-name=\"%2\">%3</h2>\n")
                        .arg(id).arg(st.name.toHtmlEscaped()).arg(title);
            if (!st.text.isEmpty())
                html += QString("      <p class=\"page-text\">%1</p>\n").arg(st.text.toHtmlEscaped());
            html += "      <hr class=\"divider\">\n";
            html += "      <div class=\"btn-row\">\n";
            if (st.returnButton)
                html += QString("        <button class=\"btn btn-secondary\" onclick=\"history.go(-1)\">← %1</button>\n")
                            .arg(st.returnButtonText.isEmpty() ? "Назад" : st.returnButtonText.toHtmlEscaped());
            if (st.nextButton || !st.nextButtonText.isEmpty()) {
                html += QString("        <button class=\"btn btn-primary\" onclick=\"nextPage(%1)\">%2 →</button>\n")
                            .arg(id)
                            .arg(st.nextButtonText.isEmpty() ? "Далее" : st.nextButtonText.toHtmlEscaped());
            } else {
                html += QString("        <button class=\"btn btn-primary\" onclick=\"nextPage(%1)\">Далее →</button>\n").arg(id);
            }
            html += "      </div>\n";
        }

        else if (t == Node::oneOfMany) {
            html += "      <div class=\"page-badge\">Вопрос</div>\n";
            QString title = st.name.isEmpty() ? "Вопрос" : st.name.toHtmlEscaped();
            html += QString("      <h2 class=\"page-title\" id=\"title-%1\" data-name=\"%2\">%3</h2>\n")
                        .arg(id).arg(st.name.toHtmlEscaped()).arg(title);
            if (!st.text.isEmpty())
                html += QString("      <p class=\"page-text\">%1</p>\n").arg(st.text.toHtmlEscaped());

            // Варианты ответов (q1–q4, пустые пропускаем)
            QStringList opts = {st.q1, st.q2, st.q3, st.q4};
            html += "      <ul class=\"options-list\">\n";
            int optIdx = 0;
            for (const QString& opt : opts) {
                if (opt.trimmed().isEmpty()) continue;
                html += QString("        <li class=\"option-item\">"
                                "<label>"
                                "<input type=\"radio\" name=\"q%1\" value=\"%2\"> %3"
                                "</label></li>\n")
                            .arg(id)
                            .arg(opt.toHtmlEscaped())
                            .arg(opt.toHtmlEscaped());
                optIdx++;
            }
            if (optIdx == 0) {
                html += QString("        <li class=\"option-item\"><label><input type=\"radio\" name=\"q%1\" value=\"Да\"> Да</label></li>\n").arg(id);
                html += QString("        <li class=\"option-item\"><label><input type=\"radio\" name=\"q%1\" value=\"Нет\"> Нет</label></li>\n").arg(id);
            }
            html += "      </ul>\n";
            html += QString("      <div class=\"answer-feedback\" id=\"fb-%1\"></div>\n").arg(id);
            html += "      <hr class=\"divider\">\n";
            html += "      <div class=\"btn-row\">\n";
            if (st.returnButton)
                html += QString("        <button class=\"btn btn-secondary\" onclick=\"prevPage(%1)\">← %2</button>\n")
                            .arg(id > 0 ? id - 1 : 0)
                            .arg(st.returnButtonText.isEmpty() ? "Назад" : st.returnButtonText.toHtmlEscaped());
            html += QString("        <button class=\"btn btn-primary\" onclick=\"nextPage(%1)\">%2 →</button>\n")
                        .arg(id)
                        .arg(st.nextButtonText.isEmpty() ? "Ответить" : st.nextButtonText.toHtmlEscaped());
            html += "      </div>\n";
        }

        else if (t == Node::Action) {
            html += "      <!-- Action node: auto-redirect -->\n";
        }

        else if (t == Node::Server) {
            html += "      <!-- Server node: auto-submit -->\n";
            html += "      <div class=\"page-badge\">Отправка данных</div>\n";
            html += "      <h2 class=\"page-title\">Сохранение ответов...</h2>\n";
            html += "      <p class=\"page-text\">Пожалуйста, подождите. Ваши ответы сохраняются.</p>\n";
        }

        else {
            html += "      <div class=\"page-badge\">Страница</div>\n";
            QString title = st.name.isEmpty() ? "Страница" : st.name.toHtmlEscaped();
            html += QString("      <h2 class=\"page-title\" id=\"title-%1\" data-name=\"%2\">%3</h2>\n")
                        .arg(id).arg(st.name.toHtmlEscaped()).arg(title);
            if (!st.text.isEmpty())
                html += QString("      <p class=\"page-text\">%1</p>\n").arg(st.text.toHtmlEscaped());
            html += "      <hr class=\"divider\">\n";
            html += QString("      <div class=\"btn-row\"><button class=\"btn btn-primary\" onclick=\"nextPage(%1)\">Далее →</button></div>\n").arg(id);
        }

        html += "    </div>\n"; // .page
    }

    // ---- Страница результатов (финал) ----
    html += "    <div class=\"page\" id=\"result-page\">\n";
    html += "      <div class=\"result-icon\">🎉</div>\n";
    html += "      <div class=\"result-score\" id=\"result-score\"></div>\n";
    html += "      <div class=\"result-label\">правильных ответов</div>\n";
    html += "      <h2 class=\"page-title\" style=\"text-align:center\">Опрос завершён!</h2>\n";
    html += "      <p class=\"page-text\" style=\"text-align:center\">Спасибо за участие.</p>\n";
    html += "      <hr class=\"divider\">\n";
    html += "      <div class=\"btn-row\" style=\"justify-content:center\">\n";
    html += "        <button class=\"btn btn-primary\" onclick=\"location.reload()\">↺ Начать заново</button>\n";
    html += "      </div>\n";
    html += "    </div>\n";

    html += "  </div>\n"; // .survey-wrapper
    return html;
}

QString GraphicsView::typeToString(Node::Type t)
{
    switch(t) {
    case Node::TextPage:  return "textpage";
    case Node::oneOfMany: return "oneofmany";
    case Node::Action:    return "action";
    case Node::Start:     return "start";
    case Node::Server:    return "server";
    default:              return "other";
    }
}


void GraphicsView::drawLineSlot(Connector *c, QPointF pos)
{
    activeConnector = c;

    tempLine = new LineItem();
    tempLine->setZValue(1);

    QPointF startPos = c->scenePos();
    tempLine->setLine(startPos.x(), startPos.y(), startPos.x(), startPos.y());

    scene->addItem(tempLine);
}

void GraphicsView::reDrawLineSlot(QVector<Connector *> connectors)
{
    qDebug() << "reDraw";
    for(int i = 0; i < connectors.size(); ++i) {
        for (int j = 0; j < cLines.size(); ++j) {
            if(connectors.at(i) == cLines.at(j).first || connectors.at(i) == cLines.at(j).second){
                cLines[j].line->setLine(cLines[j].first->scenePos().x(), cLines[j].first->scenePos().y(),
                                        cLines[j].second->scenePos().x(), cLines[j].second->scenePos().y());
            }
        }
    }
}

void GraphicsView::changeCurrentPageSlot(QString p, Node* n)
{
    Rframe->changeCurrentPage(p, n);
    if(p == "Action"){
        Node* node = getPrevNode(n);
        if(!node) return;
        QStringList lst;
        do{
            if(node && node->getNodetype() == Node::oneOfMany){
                lst.append(node->getName());
            } else {
                qDebug() << "nullptr";
            }
            node = getPrevNode(node);
        } while(node);
        n->setInputBox(lst);
        return;
    }
}

void GraphicsView::removeNodeFromConnectorSlot(Connector *c)
{
    for (int j = 0; j < nodes.size(); ++j) {
        QList<Connector*> conList  = nodes[j]->getConnectors();
        if(conList.contains(c)){
            removeNode(nodes[j]);
            scene->removeItem(nodes[j]);
            nodes.remove(j);
            return;
        }
    }
}

void GraphicsView::reConnectFromConnectorSlot(Connector *c)
{
    for(int i = 0; i < cLines.size(); ++i){
        if(cLines[i].first == c || cLines[i].second == c){
            scene->removeItem(cLines[i].line);
            cLines.removeAt(i);
            return;
        }
    }
}

void GraphicsView::removeNode(Node *n)
{
    qDebug() << "del";
    QList<Connector*> c = n->getConnectors();
    if(n->getNodetype() == Node::Start){
        startNode = nullptr;
    }
    for (auto t : c) {
        for(int i = 0; i < cLines.size(); ++i){
            if(cLines[i].first == t || cLines[i].second == t){
                scene->removeItem(cLines[i].line);
                cLines.removeAt(i);
            }
        }
    }
    scene->removeItem(n);
}

Node *GraphicsView::getPrevNode(Node *n)
{
    Connector* Left = nullptr;
    QList<Connector*> conList = n->getConnectors();
    for (int i = 0; i < conList.size(); ++i) {
        if(!conList[i]->getRight()){
            Left = conList[i];
            break;
        }
    }
    if (!Left) return nullptr;
    Node* node = nullptr;
    for(int i = 0; i < cLines.size(); ++i){
        if(cLines[i].first == Left){
            node = cLines[i].second->getNode();
            qDebug() << "Node find";
            return node;
        } else if(cLines[i].second == Left){
            node = cLines[i].first->getNode();
            qDebug() << "Node find";
            return node;
        }
    }
    return node;
}

QList<Node*> GraphicsView::getNextNode(Node *n)
{
    QList<Node*> lst;
    QList<Connector*> conList = n->getConnectors();

    for (int i = 0; i < conList.size(); ++i) {
        if(conList[i]->getRight()){
            Connector* right = conList[i];
            for(int j = 0; j < cLines.size(); ++j){
                if(cLines[j].first == right){
                    lst.append(cLines[j].second->getNode());
                } else if(cLines[j].second == right){
                    lst.append(cLines[j].first->getNode());
                }
            }
        }
    }
    return lst;
}

Node *GraphicsView::getStartNode() const
{
    return startNode;
}


static QString nodeTypeToXmlString(Node::Type t)
{
    switch (t) {
    case Node::Simple:      return "Simple";
    case Node::HeaderBlue:  return "HeaderBlue";
    case Node::Note:        return "Note";
    case Node::Random:      return "Random";
    case Node::Action:      return "Action";
    case Node::Start:       return "Start";
    case Node::TextPage:    return "TextPage";
    case Node::oneOfMany:   return "oneOfMany";
    case Node::Server:      return "Server";
    default:                return "Simple";
    }
}

static Node::Type xmlStringToNodeType(const QString& s)
{
    if (s == "HeaderBlue") return Node::HeaderBlue;
    if (s == "Note")       return Node::Note;
    if (s == "Random")     return Node::Random;
    if (s == "Action")     return Node::Action;
    if (s == "Start")      return Node::Start;
    if (s == "TextPage")   return Node::TextPage;
    if (s == "oneOfMany")  return Node::oneOfMany;
    if (s == "Server")     return Node::Server;
    return Node::Simple;
}


bool GraphicsView::saveProject(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Ошибка сохранения",
                              "Не удалось открыть файл для записи:\n" + filePath);
        return false;
    }

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("NodeEditorProject");
    xml.writeAttribute("version", "1");


    xml.writeStartElement("Nodes");
    for (int i = 0; i < nodes.size(); ++i) {
        Node* n = nodes[i];
        xml.writeStartElement("Node");
        xml.writeAttribute("id",   QString::number(i));
        xml.writeAttribute("type", nodeTypeToXmlString(n->getNodetype()));
        xml.writeAttribute("x",    QString::number(n->scenePos().x()));
        xml.writeAttribute("y",    QString::number(n->scenePos().y()));

        Node::Settings st = n->settings();
        xml.writeTextElement("Name",            st.name);
        xml.writeTextElement("Text",            st.text);
        xml.writeTextElement("NextButton",      st.nextButton      ? "1" : "0");
        xml.writeTextElement("ReturnButton",    st.returnButton    ? "1" : "0");
        xml.writeTextElement("ReturnButtonText",st.returnButtonText);
        xml.writeTextElement("NextButtonText",  st.nextButtonText);
        xml.writeTextElement("Test",            st.test ? "1" : "0");
        xml.writeTextElement("Q1",              st.q1);
        xml.writeTextElement("Q2",              st.q2);
        xml.writeTextElement("Q3",              st.q3);
        xml.writeTextElement("Q4",              st.q4);
        xml.writeTextElement("CorrectAnswer",   st.correctAnswer);
        xml.writeTextElement("LocalSave",       st.localSave  ? "1" : "0");
        xml.writeTextElement("ServerSave",      st.serverSave ? "1" : "0");
        xml.writeTextElement("Path",            st.path);
        xml.writeTextElement("Url",             st.url);
        xml.writeTextElement("Format",          st.format);


        QVector<Connector*> cons = n->getConnectors();
        xml.writeStartElement("Connectors");
        for (int ci = 0; ci < cons.size(); ++ci) {
            xml.writeStartElement("Connector");
            xml.writeAttribute("index", QString::number(ci));
            xml.writeAttribute("right", cons[ci]->getRight() ? "1" : "0");
            xml.writeEndElement(); // Connector
        }
        xml.writeEndElement(); // Connectors

        //  IfElseWidge
        if (n->getNodetype() == Node::Action && n->getWdg()) {
            xml.writeStartElement("IfElse");
            xml.writeTextElement("InputBox",  n->getWdg()->getInputBoxText());
            xml.writeTextElement("Operator",  n->getWdg()->getOperatorText());
            xml.writeTextElement("Value",     n->getWdg()->getValueText());
            xml.writeEndElement(); // IfElse
        }

        xml.writeEndElement(); // Node
    }
    xml.writeEndElement(); // Nodes


    xml.writeStartElement("Connections");
    for (const ConnectorLine& cl : cLines) {

        int firstNodeId = -1, firstConIdx = -1;
        int secondNodeId = -1, secondConIdx = -1;

        for (int ni = 0; ni < nodes.size(); ++ni) {
            QVector<Connector*> cons = nodes[ni]->getConnectors();
            for (int ci = 0; ci < cons.size(); ++ci) {
                if (cons[ci] == cl.first)  { firstNodeId  = ni; firstConIdx  = ci; }
                if (cons[ci] == cl.second) { secondNodeId = ni; secondConIdx = ci; }
            }
        }

        if (firstNodeId < 0 || secondNodeId < 0)
            continue;

        xml.writeStartElement("Connection");
        xml.writeAttribute("firstNode",  QString::number(firstNodeId));
        xml.writeAttribute("firstCon",   QString::number(firstConIdx));
        xml.writeAttribute("secondNode", QString::number(secondNodeId));
        xml.writeAttribute("secondCon",  QString::number(secondConIdx));
        xml.writeEndElement(); // Connection
    }
    xml.writeEndElement(); // Connections

    xml.writeEndElement(); // NodeEditorProject
    xml.writeEndDocument();
    file.close();

    m_currentFilePath = filePath;
    return true;
}


Node* GraphicsView::createNodeFromXml(QXmlStreamReader& xml,
                                      qreal x, qreal y,
                                      Node::Type ntype,
                                      QString& ifElseInput,
                                      QString& ifElseOp,
                                      QString& ifElseValue)
{
    QString iconPath;
    switch (ntype) {
    case Node::Start:      iconPath = ":/res/icons/start.png"; break;
    case Node::Action:     iconPath = ":/res/icons/if.png";    break;
    case Node::TextPage:   iconPath = ":/res/icons/page.png";  break;
    case Node::oneOfMany:  iconPath = ":/res/icons/radio.png"; break;
    case Node::Server:     iconPath = ":/res/icons/start.png"; break;
    default:               iconPath = ":/res/icons/text.png";  break;
    }
    QPixmap icon(iconPath);

    Node* node = nullptr;
    Node::Settings st;
    ifElseInput = ifElseOp = ifElseValue = "";

    while (xml.readNextStartElement()) {
        const QString tag = xml.name().toString();

        if (tag == "Name")             { st.name              = xml.readElementText(); }
        else if (tag == "Text")        { st.text              = xml.readElementText(); }
        else if (tag == "NextButton")  { st.nextButton        = xml.readElementText() == "1"; }
        else if (tag == "ReturnButton"){ st.returnButton      = xml.readElementText() == "1"; }
        else if (tag == "ReturnButtonText") { st.returnButtonText = xml.readElementText(); }
        else if (tag == "NextButtonText")   { st.nextButtonText  = xml.readElementText(); }
        else if (tag == "Test")        { st.test              = xml.readElementText() == "1"; }
        else if (tag == "Q1")          { st.q1                = xml.readElementText(); }
        else if (tag == "Q2")          { st.q2                = xml.readElementText(); }
        else if (tag == "Q3")          { st.q3                = xml.readElementText(); }
        else if (tag == "Q4")          { st.q4                = xml.readElementText(); }
        else if (tag == "CorrectAnswer"){ st.correctAnswer    = xml.readElementText(); }
        else if (tag == "LocalSave")   { st.localSave         = xml.readElementText() == "1"; }
        else if (tag == "ServerSave")  { st.serverSave        = xml.readElementText() == "1"; }
        else if (tag == "Path")        { st.path              = xml.readElementText(); }
        else if (tag == "Url")         { st.url               = xml.readElementText(); }
        else if (tag == "Format")      { st.format            = xml.readElementText(); }
        else if (tag == "Connectors")  { xml.skipCurrentElement(); }
        else if (tag == "IfElse") {
            while (xml.readNextStartElement()) {
                const QString itag = xml.name().toString();
                if      (itag == "InputBox") ifElseInput = xml.readElementText();
                else if (itag == "Operator") ifElseOp    = xml.readElementText();
                else if (itag == "Value")    ifElseValue = xml.readElementText();
                else xml.skipCurrentElement();
            }
        }
        else { xml.skipCurrentElement(); }
    }


    if (ntype == Node::Action) {
        node = new Node(Node::Action, icon);
        IfElseWidget* widget = new IfElseWidget;
        QGraphicsProxyWidget* proxy = scene->addWidget(widget);
        proxy->setParentItem(node);
        proxy->setPos(10, 45);
        widget->setFixedWidth(240);
        node->setWdg(widget);

        Connector* left = new Connector(node);
        left->setPos(0, 55); left->setRight(false);
        node->addConnector(left); left->setNode(node);

        Connector* right = new Connector(node);
        right->setPos(260, 55); right->setRight(true);
        node->addConnector(right); right->setNode(node);

        Connector* rightFalse = new Connector(node);
        rightFalse->setPos(260, 85); rightFalse->setRight(true);
        node->addConnector(rightFalse); rightFalse->setNode(node);

        connect(left,       &Connector::drawLineSignal,   this, &GraphicsView::drawLineSlot);
        connect(right,      &Connector::drawLineSignal,   this, &GraphicsView::drawLineSlot);
        connect(rightFalse, &Connector::drawLineSignal,   this, &GraphicsView::drawLineSlot);
        connect(left,       &Connector::deleteNodeSignal, this, &GraphicsView::removeNodeFromConnectorSlot);
        connect(right,      &Connector::deleteNodeSignal, this, &GraphicsView::removeNodeFromConnectorSlot);
        connect(rightFalse, &Connector::deleteNodeSignal, this, &GraphicsView::removeNodeFromConnectorSlot);
        connect(left,       &Connector::reConnectSignal,  this, &GraphicsView::reConnectFromConnectorSlot);
        connect(right,      &Connector::reConnectSignal,  this, &GraphicsView::reConnectFromConnectorSlot);
        connect(rightFalse, &Connector::reConnectSignal,  this, &GraphicsView::reConnectFromConnectorSlot);

    } else if (ntype == Node::Start) {
        node = new Node(Node::Start, icon);
        Connector* right = new Connector(node);
        right->setPos(80, 20); right->setRight(true);
        node->addConnector(right); right->setNode(node);
        connect(right, &Connector::drawLineSignal,   this, &GraphicsView::drawLineSlot);
        connect(right, &Connector::deleteNodeSignal, this, &GraphicsView::removeNodeFromConnectorSlot);
        connect(right, &Connector::reConnectSignal,  this, &GraphicsView::reConnectFromConnectorSlot);
        startNode = node;

    } else if (ntype == Node::TextPage) {
        node = new Node(Node::TextPage, icon);
        Connector* right = new Connector(node); right->setPos(200,45); right->setRight(true);
        node->addConnector(right); right->setNode(node);
        Connector* left  = new Connector(node); left->setPos(0,45);   left->setRight(false);
        node->addConnector(left);  left->setNode(node);
        connect(right, &Connector::drawLineSignal,   this, &GraphicsView::drawLineSlot);
        connect(right, &Connector::deleteNodeSignal, this, &GraphicsView::removeNodeFromConnectorSlot);
        connect(left,  &Connector::drawLineSignal,   this, &GraphicsView::drawLineSlot);
        connect(left,  &Connector::deleteNodeSignal, this, &GraphicsView::removeNodeFromConnectorSlot);
        connect(right, &Connector::reConnectSignal,  this, &GraphicsView::reConnectFromConnectorSlot);
        connect(left,  &Connector::reConnectSignal,  this, &GraphicsView::reConnectFromConnectorSlot);

    } else if (ntype == Node::oneOfMany) {
        node = new Node(Node::oneOfMany, icon);
        Connector* right = new Connector(node); right->setPos(200,45); right->setRight(true);
        node->addConnector(right); right->setNode(node);
        Connector* left  = new Connector(node); left->setPos(0,45);   left->setRight(false);
        node->addConnector(left);  left->setNode(node);
        connect(right, &Connector::drawLineSignal,   this, &GraphicsView::drawLineSlot);
        connect(left,  &Connector::drawLineSignal,   this, &GraphicsView::drawLineSlot);
        connect(right, &Connector::deleteNodeSignal, this, &GraphicsView::removeNodeFromConnectorSlot);
        connect(left,  &Connector::deleteNodeSignal, this, &GraphicsView::removeNodeFromConnectorSlot);
        connect(right, &Connector::reConnectSignal,  this, &GraphicsView::reConnectFromConnectorSlot);
        connect(left,  &Connector::reConnectSignal,  this, &GraphicsView::reConnectFromConnectorSlot);
        numQuestion++;

    } else if (ntype == Node::Server) {
        node = new Node(Node::Server, icon);
        Connector* right = new Connector(node); right->setPos(120,20); right->setRight(true);
        Connector* left  = new Connector(node); left->setPos(0,20);    left->setRight(false); // FIX: вход
        node->addConnector(right); right->setNode(node);
        node->addConnector(left);  left->setNode(node);
        connect(right, &Connector::drawLineSignal,   this, &GraphicsView::drawLineSlot);
        connect(right, &Connector::deleteNodeSignal, this, &GraphicsView::removeNodeFromConnectorSlot);
        connect(right, &Connector::reConnectSignal,  this, &GraphicsView::reConnectFromConnectorSlot);
        connect(left,  &Connector::drawLineSignal,   this, &GraphicsView::drawLineSlot);
        connect(left,  &Connector::deleteNodeSignal, this, &GraphicsView::removeNodeFromConnectorSlot);
        connect(left,  &Connector::reConnectSignal,  this, &GraphicsView::reConnectFromConnectorSlot);

    } else {
        node = new Node(ntype, icon);
    }

    node->setSettings(st);
    scene->addItem(node);
    nodes.push_back(node);
    node->setPos(x, y);

    connect(node, &Node::moveSignal,   this, &GraphicsView::reDrawLineSlot);
    connect(node, &Node::pressSignal,  this, &GraphicsView::changeCurrentPageSlot);

    return node;
}

bool GraphicsView::loadProject(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Ошибка открытия",
                              "Не удалось открыть файл:\n" + filePath);
        return false;
    }

    scene->clear();
    nodes.clear();
    cLines.clear();
    startNode = nullptr;
    numQuestion = 0;

    QXmlStreamReader xml(&file);


    QMap<int, Node*> nodeMap;
    struct IfElseData { QString input, op, value; };
    QMap<int, IfElseData> ifElseMap;

    struct ConnectRequest {
        int firstNodeId, firstConIdx;
        int secondNodeId, secondConIdx;
    };
    QList<ConnectRequest> connectRequests;

    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token != QXmlStreamReader::StartElement) continue;

        const QString tag = xml.name().toString();

        if (tag == "Node") {
            int id        = xml.attributes().value("id").toInt();
            Node::Type nt = xmlStringToNodeType(xml.attributes().value("type").toString());
            qreal x       = xml.attributes().value("x").toDouble();
            qreal y       = xml.attributes().value("y").toDouble();

            QString ileInput, ileOp, ileVal;
            Node* n = createNodeFromXml(xml, x, y, nt, ileInput, ileOp, ileVal);
            if (n) {
                nodeMap[id] = n;
                if (!ileInput.isEmpty() || !ileOp.isEmpty() || !ileVal.isEmpty()) {
                    ifElseMap[id] = { ileInput, ileOp, ileVal };
                }
            }
        }


        else if (tag == "Connection") {
            ConnectRequest req;
            req.firstNodeId  = xml.attributes().value("firstNode").toInt();
            req.firstConIdx  = xml.attributes().value("firstCon").toInt();
            req.secondNodeId = xml.attributes().value("secondNode").toInt();
            req.secondConIdx = xml.attributes().value("secondCon").toInt();
            connectRequests.append(req);
            xml.skipCurrentElement();
        }
    }

    if (xml.hasError()) {
        QMessageBox::critical(nullptr, "Ошибка XML",
                              "Ошибка разбора файла:\n" + xml.errorString());
        file.close();
        return false;
    }
    file.close();

    for (auto it = ifElseMap.begin(); it != ifElseMap.end(); ++it) {
        Node* n = nodeMap.value(it.key(), nullptr);
        if (n && n->getWdg()) {
            QStringList ops = {"==", "!=", ">", "<", ">=", "<="};
            int opIdx = ops.indexOf(it.value().op);
            if (opIdx < 0) opIdx = 0;
            n->getWdg()->setBoxValues(opIdx, {it.value().input}, 0);
        }
    }
    for (const ConnectRequest& req : connectRequests) {
        Node* firstNode  = nodeMap.value(req.firstNodeId,  nullptr);
        Node* secondNode = nodeMap.value(req.secondNodeId, nullptr);
        if (!firstNode || !secondNode) continue;

        QVector<Connector*> firstCons  = firstNode->getConnectors();
        QVector<Connector*> secondCons = secondNode->getConnectors();
        if (req.firstConIdx  >= firstCons.size())  continue;
        if (req.secondConIdx >= secondCons.size()) continue;

        Connector* c1 = firstCons[req.firstConIdx];
        Connector* c2 = secondCons[req.secondConIdx];

        LineItem* line = new LineItem();
        QPen pen(QColor("#3B82F6"), 2);
        pen.setStyle(Qt::SolidLine);
        line->setPen(pen);
        line->setLine(c1->scenePos().x(), c1->scenePos().y(),
                      c2->scenePos().x(), c2->scenePos().y());
        line->setZValue(-1);
        scene->addItem(line);

        cLines.push_back({c1, c2, line});
    }

    m_currentFilePath = filePath;
    return true;
}


bool GraphicsView::writeXmlFile(QFile &file)
{
    return saveProject(file.fileName());
}

void GraphicsView::setRframe(RightFrame *newRframe)
{
    Rframe = newRframe;
    Rframe->setView(this);
}

void GraphicsView::removeNodeFromFrame(Node *n)
{
    qDebug() << "del";
    QList<Connector*> c = n->getConnectors();
    if(n->getNodetype() == Node::Start){
        startNode = nullptr;
    }
    for (auto t : c) {
        for(int i = 0; i < cLines.size(); ++i){
            if(cLines[i].first == t || cLines[i].second == t){
                scene->removeItem(cLines[i].line);
                cLines.removeAt(i);
            }
        }
    }
    scene->removeItem(n);
    nodes.removeOne(n);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton && spacePressed){
        QPoint delta = event->pos() - lastMousePos;
        lastMousePos = event->pos();
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        expandSceneRectIfNeeded();
        event->accept();
        return;
    }
    else if (activeConnector && tempLine) {
        QPointF scenePos = mapToScene(event->pos());
        QPointF startPos = activeConnector->scenePos();
        tempLine->setLine(startPos.x(), startPos.y(), scenePos.x(), scenePos.y());
    }
    else if (m_nodeDragging && (event->buttons() & Qt::LeftButton) && !m_dragStartPositions.isEmpty()) {
        QPointF currentScene = mapToScene(event->pos());
        QPointF delta = currentScene - m_dragStartScene;
        for (auto it = m_dragStartPositions.begin(); it != m_dragStartPositions.end(); ++it) {
            it.key()->setPos(it.value() + delta);
        }
        for (Node* n : nodes) {
            if (n->isSelected()) {
                reDrawLineSlot(n->getConnectors());
            }
        }
        event->accept();
        return;

    } else if (m_selecting && m_selectRect) {
        QPointF curPos = mapToScene(event->pos());
        qreal x = qMin(m_selectOrigin.x(), curPos.x());
        qreal y = qMin(m_selectOrigin.y(), curPos.y());
        qreal w = qAbs(curPos.x() - m_selectOrigin.x());
        qreal h = qAbs(curPos.y() - m_selectOrigin.y());
        m_selectRect->setRect(x, y, w, h);
    }
    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_nodeDragging && event->button() == Qt::LeftButton) {
        m_nodeDragging = false;
        for (Node* n : nodes) {
            n->setFlag(QGraphicsItem::ItemIsMovable, true);
        }
        m_dragStartPositions.clear();
    }

    if (activeConnector && tempLine) {

        QList<QGraphicsItem*> itemsList = items(event->pos());

        Connector *target = nullptr;
        for (QGraphicsItem* item : itemsList) {
            target = dynamic_cast<Connector*>(item);
            if (target && target != activeConnector && target->getNode() != activeConnector->getNode()) {
                break;
            }
            target = nullptr;
        }

        if (target) {
            LineItem* finalLine = new LineItem();
            QPen pen(QColor("#3B82F6"), 2);
            pen.setStyle(Qt::SolidLine);
            finalLine->setPen(pen);

            finalLine->setLine(activeConnector->scenePos().x(), activeConnector->scenePos().y(),
                               target->scenePos().x(), target->scenePos().y());

            scene->addItem(finalLine);
            finalLine->setZValue(-1);

            cLines.push_back({activeConnector, target, finalLine});
            qDebug() << "Connection!";
        } else {
            qDebug() << "Not Connection";
        }

        scene->removeItem(tempLine);
        delete tempLine;
        tempLine = nullptr;
        activeConnector = nullptr;

    } else if (m_selecting && m_selectRect && event->button() == Qt::LeftButton) {
        QRectF selRect = m_selectRect->rect();

        //Сброс выдел.
        for (Node* n : nodes) {
            n->setSelected(false);
        }

        //нвое выделение
        for (Node* n : nodes) {
            QRectF nodeRect = n->mapToScene(n->boundingRect()).boundingRect();
            if (selRect.intersects(nodeRect)) {
                n->setSelected(true);
            }
        }

        scene->removeItem(m_selectRect);
        delete m_selectRect;
        m_selectRect = nullptr;
        m_selecting  = false;
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete){
        for (int j = nodes.size()-1; j >= 0; --j) {
            if(nodes[j]->isSelected()){
                removeNode(nodes[j]);
                scene->removeItem(nodes[j]);
                nodes.remove(j);
            }
        }
    }else if(event->key() == Qt::Key_Space && !event->isAutoRepeat()){
        spacePressed = true;
        setCursor(Qt::OpenHandCursor);
        qDebug() << "space";
    }
    QGraphicsView::keyPressEvent(event);
}

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space && event->isAutoRepeat()){
        return;
    }
    if(event->key() == Qt::Key_Space){
        spacePressed = false;
        setCursor(Qt::ArrowCursor);
    }
}

void GraphicsView::expandSceneRectIfNeeded()
{
    QRectF visible = mapToScene(viewport()->rect()).boundingRect();
    QRectF current = sceneRect();

    const qreal margin = 2000.0;
    qreal left   = qMin(current.left(),   visible.left()   - margin);
    qreal top    = qMin(current.top(),    visible.top()    - margin);
    qreal right  = qMax(current.right(),  visible.right()  + margin);
    qreal bottom = qMax(current.bottom(), visible.bottom() + margin);

    setSceneRect(left, top, right - left, bottom - top);
}

void GraphicsView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    expandSceneRectIfNeeded();
}
