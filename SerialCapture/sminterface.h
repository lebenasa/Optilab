#ifndef SMINTERFACE_H
#define SMINTERFACE_H

#include <QObject>

// Intermediate data handler between Stepper and Model
class SMInterface : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QPoint cellPos MEMBER m_index NOTIFY cellPosChanged)
	QSizeF m_size;			// Current calibrated size
	double m_overlap;		// Overlap between cells, default to 25%
	int m_rows, m_cols;		// Rows and cols based on limit and frame size
	QPoint m_index;			// Current position in 2D index
	QPointF m_pos;			// Current position in stepper coordinate
	// This class stores all possible coordinate
	std::vector<QPointF> m_grid;
	int m_profile;
public:
	SMInterface(int profileIndex = 0, QObject *parent = 0);
	~SMInterface();

	int cols() const { return m_cols; }
	int rows() const { return m_rows; }
	QPoint currentPos() const { return m_index; }
	double overlap() const { return m_overlap; }
	void setOverlap(double& ovl);

	QPoint coordToIndex(const QPointF& pos) const;
	QPointF indexToCoord(const QPoint& pos) const;

public slots:
	// Load calibration profile from registry and recalculate
	void loadProfile(int index);

	// Update status
	void updatePos(const QPointF &pos);
	// Move stepper to nearest grid node
	void alignToGrid();	// in mm
	// Move stepper to certain position in grid
	void moveTo(const QPoint& index);	// in 2D index

signals:
	void cellPosChanged(const QPoint& npos);
	void stepperMoveTo(const QPointF& newPos);	// in mm
};

#endif // SMINTERFACE_H
